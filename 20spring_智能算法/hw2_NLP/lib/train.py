import torch
import torch.nn as nn
import torch.utils.data as Data
from torch.utils.data.sampler import WeightedRandomSampler
from torch.optim import Adam, SGD

import argparse
import os

from dataloader import *
from utils import *
from model import LSTM
from timer import Timer

parser = argparse.ArgumentParser(description='Sentiment Analysis Training')
parser.add_argument('--epoch', default=100, type=int, help='epoch size')
parser.add_argument('--batch', default=128, type=int, help='batch size')
parser.add_argument('--embed', default=50, type=int, help='embedding size')
parser.add_argument('--init_LR', default=1e-4, type=float, help='initial learning rate')
parser.add_argument('--out_dir', default='Result', type=str, help='save dir')
parser.add_argument('--drop_rate', default=0, type=float, help='dropout rate')
parser.add_argument('--num_layers', default=2, type=int, help='num layers')
parser.add_argument('--load_embd', default=None, type=str, help='pretrain model path')
parser.add_argument('--freeze', default=False, type=bool, help='freeze pretrain layer')
parser.add_argument('--weighted', default=False, type=bool, help='use weighted sampler')
parser.add_argument('--split_rate', default=0.8, type=float, help='split rate')

args = parser.parse_args()
print("call with args:", args)

epoch_size = args.epoch
batch_size = args.batch
embed_size = args.embed
init_LR = args.init_LR
out_dir = args.out_dir
drop_rate = args.drop_rate
embd_path = args.load_embd
num_layers = args.num_layers
freeze = args.freeze
weighted = args.weighted
split_rate = args.split_rate
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

def main():
    train_dir = './data/train.tsv'
    train_x, train_y = load_data(train_dir)
    print('Load train data with size', len(train_x))
    
    init_output_log(out_dir)
    _dict = build_dict(train_x, out_dir)
    dim_in = len(_dict)
    dim_out = 5
    train_x = word2index(train_x, _dict)
    train_x, lengths = Padding(train_x)
    train_y = torch.LongTensor(train_y)

    train_x, train_y, train_l, valid_x, valid_y, valid_l = \
        Split_data(train_x, train_y, lengths, split_rate)
    train_set = Data.TensorDataset(train_x, train_y, train_l)
    if weighted==True:
        samples_weight = get_samples_weight(train_y)
        sampler = WeightedRandomSampler(samples_weight, len(samples_weight))
    else: sampler = None
    train_loader = Data.DataLoader(
		dataset = train_set,
		batch_size = batch_size,
		shuffle = False,
		num_workers = 0,
        sampler = sampler
	)
    valid_set = Data.TensorDataset(valid_x, valid_y, valid_l)
    valid_loader = Data.DataLoader(
		dataset = valid_set,
		batch_size = batch_size,
		shuffle = False,
		num_workers = 0
	)
    print('Dataset load done')

    if embd_path is not None:
        pretrain_embd = load_embadding(embd_path, embed_size, _dict)
    else: pretrain_embd = None
    
    model = LSTM(dim_in, dim_out, \
                input_size=embed_size, \
                device=device, drop_rate=drop_rate, \
                pretrain_embd = pretrain_embd,
                num_layers = num_layers,
                freeze = freeze ).to(device)
    optimizer = Adam(model.parameters(), lr=init_LR, weight_decay=1e-4)
    loss_fn = nn.CrossEntropyLoss()
    print('Model load done')

    print('Trainning start')
    timer = Timer(epoch_size)
    min_loss = 100
    max_val_acc = 0
    Count = 0
    for epoch in range(epoch_size):
        if Count>=5:
            Count = 0
            lr = optimizer.param_groups[0]['lr']
            Decay_LR(optimizer, 0.1)
            lr_new = optimizer.param_groups[0]['lr']
            assert lr != lr_new

        train_loss = 0
        train_acc = 0
        iter_size = 0
        model.train()
        for step, (inputs, labels, lengths) in enumerate(train_loader):
            inputs = inputs.to(device)
            labels = labels.to(device)
            lengths = lengths.to(device)
            outputs = model(inputs, lengths)
            loss = loss_fn(outputs, labels)
            loss.backward()
            optimizer.step()
            predict = outputs.argmax(dim=1)
            train_acc += len(np.where(predict == labels)[0])
            train_loss += loss
            iter_size += 1
            if (step+1)%250 ==0:
                print("[Iter] epoch %d, iter %d, loss %f"%(epoch, step, loss))
        train_loss /= iter_size
        train_acc /= len(train_x)
        print("[Epoch] epoch %d, train_loss %f, train_acc %f"%(epoch, train_loss, train_acc))
        save_data(out_dir, train_loss, 'train_loss.txt')
        save_data(out_dir, train_acc, 'train_acc.txt')
        if train_loss < min_loss:
            min_loss = train_loss; Count = 0
        else: Count += 1

        if len(valid_x)>0:
            valid_loss = 0
            valid_acc = 0
            iter_size = 0
            with torch.no_grad():
                model.eval()
                for step, (inputs, labels, lengths) in enumerate(valid_loader):
                    inputs = inputs.to(device)
                    labels = labels.to(device)
                    lengths = lengths.to(device)
                    outputs = model(inputs, lengths)
                    loss = loss_fn(outputs, labels)
                    predict = outputs.argmax(dim=1)
                    valid_acc += len(np.where(predict == labels)[0])
                    valid_loss += loss
                    iter_size += 1
            
            valid_loss /= iter_size
            valid_acc /= len(valid_x)
            print("[Epoch] epoch %d, valid_loss %f, valid_acc %f"%(epoch, valid_loss, valid_acc))
            save_data(out_dir, valid_loss, 'valid_loss.txt')
            save_data(out_dir, valid_acc, 'valid_acc.txt')
            if valid_acc > max_val_acc:
                max_val_acc = valid_acc
                print('epoch %d, best val acc: %f'%(epoch, valid_acc))
                save_model(out_dir, model, 'best.pth')
        
        if (epoch+1)%20==0:
            save_model(out_dir, model, 'model%d.pth'%(epoch))
        timer.update()
        
    
    print('Training end')

if __name__ == "__main__":
    main()
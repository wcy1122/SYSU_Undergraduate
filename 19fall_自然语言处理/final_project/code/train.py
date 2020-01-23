import torch
import torch.nn as nn
import torch.utils.data as Data
from torch.optim import Adam, SGD

import os
import math
import argparse
import pickle
import numpy as np
from nltk.translate.bleu_score import sentence_bleu

from tools import *
from model import NMT, MyLoss

parser = argparse.ArgumentParser(description='PyTorch Machine Translate Training')
parser.add_argument('--epoch', default=600, type=int, help='epoch_size')
parser.add_argument('--batch', default=128, type=int, help='batch_size')
parser.add_argument('--embed', default=128, type=int, help='embed_size')
parser.add_argument('--lr', default=5e-3, type=float, help='learning_rate')
parser.add_argument('--lr_decay', default=0.5, type=float, help='learning_rate_decay')
parser.add_argument('--save_dir', default='result', type=str, help='save_dir')
parser.add_argument('--teach_force', default=0.4, type=float, help='teach_force')
parser.add_argument('--dataset', default='small', type=str, help='dataset_size')
parser.add_argument('--use_w2v', default=False, type=bool, help='use_w2v')
parser.add_argument('--attention', default=False, type=bool, help='use_attention')
args = parser.parse_args()
print("call with args:", args)

LR = args.lr
epoch_size = args.epoch
batch_size = args.batch
embed_size = args.embed
attention = args.attention
lr_decay = args.lr_decay
save_dir = args.save_dir
teach_force = args.teach_force
dataset = args.dataset
use_w2v = args.use_w2v
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
torch.backends.cudnn.enabled=False # ???

def padding(data, pad):
    MAXLEN = max([len(x) for x in data])
    mask = []
    ind=0
    for i in range(len(data)):
        length = len(data[i])
        data[i].extend([pad]*(MAXLEN-length))
        mask.append([1]*length+[0]*(MAXLEN-length))
    return data, mask

def Load_data(embed_size):
    if os.path.exists(fname):
        return pickle.load(open(fname,'rb'))
    raise RuntimeError('Data not found.')

def reverse(word2num):
    num2word = {}
    for key,val in word2num.items():
        num2word[val]=key
    return num2word

def data_padding(data_x, data_y):
    data_len = len(data_x)
    batch_num = math.ceil(data_len/batch_size)
    res_inputs = []
    res_outputs = []
    res_masks = []
    length = np.array([len(x) for x in data_x])
    index = np.argsort(-length)

    data_x = [data_x[pos] for pos in index]
    data_y = [data_y[pos] for pos in index]
    
    for step in range(batch_num):
        inputs = batch_iter(data_x, step, batch_size)
        labels = batch_iter(data_y, step, batch_size)
        inputs,_ = padding(inputs, train_dict['<PAD>'])
        labels,masks = padding(labels, test_dict['<PAD>'])
        res_inputs.extend(inputs)
        res_outputs.extend(labels)
        res_masks.extend(masks)
    
    return res_inputs, res_outputs, res_masks

def Calc_P(data, dim_out):
    length = 0
    count = torch.zeros(dim_out)
    for seq in data:
        length += len(seq)
        for x in seq: count[x]+=1
    count = (1-2*(count)/length)**2
    return count


if __name__ == "__main__":
    if dataset=='small':
        fname = './utils/data10000.pkl'
    else:
        fname = './utils/data100000.pkl'
    train_x, train_y, dev_x, dev_y, test_x, test_y, \
    train_dict, test_dict, w2v_train, w2v_test = Load_data(fname)
    num2word = reverse(test_dict)
    dim_in = len(train_dict)
    dim_out = len(test_dict)
    print('dataset load done.')
    # load dataset

    if use_w2v:
        w2v_train = pickle.load(open('./utils/w2vtrain.pkl','rb'))
        w2v_test = pickle.load(open('./utils/w2vtest.pkl','rb'))
        model = NMT(dim_in, dim_out, 
                    w2v_train=w2v_train,
                    w2v_test=w2v_test, 
                    ues_attention=attention,
                    ratio=teach_force
                    ).to(device)
    else:
        model = NMT(dim_in, dim_out, 
                    ues_attention=attention,
                    ratio=teach_force
                    ).to(device)
    
    optimizer = Adam(model.parameters(), lr=LR, weight_decay=1e-4)
    weight = Calc_P(train_y, dim_out)
    Loss_fn = MyLoss(weight).to(device)
    timer = Timer(epoch_size)
    init_output_log(save_dir)
    
    print('model load done.')
    # get model

    train_len = len(train_x)
    dev_len = len(dev_x)
    train_x, train_y, train_mask = data_padding(train_x, train_y)
    dev_x, dev_y, dev_mask = data_padding(dev_x, dev_y)
    
    print('training start.')

    best_loss = 998244353.0
    for epoch in range(epoch_size):
        model.train()
        sum_loss = 0
        batch_num = math.ceil(train_len/batch_size)
        for step in range(batch_num):
            inputs = batch_iter(train_x, step, batch_size)
            labels = batch_iter(train_y, step, batch_size)
            masks = batch_iter(train_mask, step, batch_size)
            inputs = torch.LongTensor(inputs).to(device)
            labels = torch.LongTensor(labels).to(device)
            masks = torch.ByteTensor(masks).to(device)

            optimizer.zero_grad()
            outputs = model(inputs, labels)
            Loss = Loss_fn(outputs, labels, masks)
            Loss.backward()
            optimizer.step()
            sum_loss+=Loss
            
            if step%100==0:
                print('epoch %d, step %d, train_loss %f'%(epoch, step, Loss))
        
        avg_loss = sum_loss / batch_num
        print('[epoch %d, train_loss %f]'%(epoch, avg_loss))
        save_data(avg_loss, 'train_loss.txt')

        if avg_loss<best_loss:
            best_loss = avg_loss; count = 0
        else: count += 1
        if count==5:
            lr = optimizer.param_groups[0]['lr'] * lr_decay
            print("{Learning rate decay to %f}"%(lr))
            for param_group in optimizer.param_groups:
                param_group['lr'] = lr
            if lr<1e-6: break
            count = 0
        # training
        
        with torch.no_grad():
            model.eval()
            sum_loss = 0
            batch_num = math.ceil(dev_len/batch_size)
            for step in range(batch_num):
                inputs = batch_iter(dev_x, step, batch_size)
                labels = batch_iter(dev_y, step, batch_size)
                masks = batch_iter(dev_mask, step, batch_size)
                inputs = torch.LongTensor(inputs).to(device)
                labels = torch.LongTensor(labels).to(device)
                masks = torch.ByteTensor(masks).to(device)

                outputs = model(inputs, labels)
                Loss = Loss_fn(outputs, labels, masks)
                sum_loss += Loss

                if step%100==0:
                    print('epoch %d, step %d, test_loss %f'%(epoch, step, Loss))
            
            avg_loss = sum_loss / batch_num
            print('[epoch %d, valid_loss %f]'%(epoch, avg_loss))
            save_data(avg_loss, 'valid_loss.txt')
        # evaluate

        if (epoch+1)%30==0:
            fname = get_dir(save_dir, 'checkpoint//model%d.pth'%(epoch))
            torch.save(model.state_dict(), fname)
        
        torch.cuda.empty_cache()
        timer.update()
    
    print('Training end!')

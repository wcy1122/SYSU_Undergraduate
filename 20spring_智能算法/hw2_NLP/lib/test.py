import torch
import torch.nn as nn
import torch.utils.data as Data
from torch.optim import Adam, SGD

import argparse
import os

from dataloader import *
from utils import *
from model import LSTM
from timer import Timer

parser = argparse.ArgumentParser(description='Sentiment Analysis Test')
parser.add_argument('--batch', default=128, type=int, help='batch size')
parser.add_argument('--embed', default=128, type=int, help='embedding size')
parser.add_argument('--in_dir', default='LSTM_50', type=str, help='load dir')
parser.add_argument('--step', default=0, type=int, help='model step')
parser.add_argument('--dataset', default='test', type=str, help='dataset type')
parser.add_argument('--sent_only', default=False, type=bool, help='test sentence only')

args = parser.parse_args()
print("call with args:", args)
batch_size = args.batch
embed_size = args.embed
in_dir = args.in_dir
_step = args.step
dataset = args.dataset
sent_only = args.sent_only
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

def main():
    if dataset=='train' or dataset=='val':
        data_dir = './data/train.tsv'
        data_x, data_y = load_data(data_dir, _type='train', _sent_only=sent_only)
    else: 
        data_dir = './data/test.tsv'
        data_x = load_data(data_dir, _type='test', _sent_only=sent_only)
    
    print('Load data with size', len(data_x))
    _dict = load_dict(in_dir)
    data_x = word2index(data_x, _dict)
    data_x, lengths = Padding(data_x)

    if dataset=='train' or dataset=='val':
        train_x, train_y, train_l, valid_x, valid_y, valid_l = \
            Split_data(data_x, data_y, lengths, 0.8)
        if dataset=='train': 
            data_x, data_y, lengths = train_x, train_y, train_l
        if dataset=='val': 
            data_x, data_y, lengths = valid_x, valid_y, valid_l
    data_set = Data.TensorDataset(data_x, lengths)
    
    test_loader = Data.DataLoader(
		dataset = data_set,
		batch_size = batch_size,
		shuffle = False,
		num_workers = 0
	)
    print('Dataset load done')

    dim_in = len(_dict)
    dim_out = 5
    model = LSTM(dim_in, dim_out, input_size=embed_size, device=device).to(device)
    if _step==0: model_name = 'best.pth'
    else: model_name = ('model%d.pth'%_step)
    model_dir = os.path.join(os.path.join(in_dir, 'checkpoint'), model_name)
    if device=='cuda': weight_dict = torch.load(model_dir)
    else: weight_dict = torch.load(model_dir, map_location='cpu')
    model.load_state_dict(weight_dict)
    print('Model load done')

    with torch.no_grad():
        predict_ans = torch.LongTensor(0).to(device)
        model.eval()
        for step, (inputs, lengths) in enumerate(test_loader):
            inputs = inputs.to(device)
            lengths = lengths.to(device)
            outputs = model(inputs, lengths)
            predict = outputs.argmax(dim=1)
            predict_ans = torch.cat((predict_ans, predict))
            if step%10==0:
                print('eval step %d'%step)
    if dataset=='test':
        write_csv(in_dir, predict_ans)
        print('Test done')
    else:
        diff_matrix = np.zeros([5,5])
        data_y = np.array(data_y)
        predict_ans = np.array(predict_ans)
        Test_Acc = 0
        for x,y in zip(predict_ans, data_y):
            diff_matrix[x,y] += 1
            Test_Acc += (x==y)
        for i in range(5):
            data_size = len(np.where(data_y == i)[0])
            if data_size>0:
                diff_matrix[:,i] /= data_size
        diff_matrix[np.where(diff_matrix<1e-3)]=0
        
        print("diff_matrix:")
        print(diff_matrix)
        print(len(data_y), Test_Acc)
        Test_Acc /= len(data_y)
        
        print("Test_Acc:", Test_Acc)

if __name__ == "__main__":
    main()
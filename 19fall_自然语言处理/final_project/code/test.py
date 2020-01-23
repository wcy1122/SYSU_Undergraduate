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

parser = argparse.ArgumentParser(description='PyTorch Machine Translate Testing')
parser.add_argument('--embed', default=128, type=int, help='embed_size')
parser.add_argument('--attention', default=True, type=bool, help='use_attention')
parser.add_argument('--dataset', default='small', type=str, help='dataset_size')
parser.add_argument('--load_dir', default='TF8//checkpoint//model239.pth', type=str, help='load_dir')
args = parser.parse_args()
print("call with args:", args)
embed_size = args.embed
attention = args.attention
load_dir = args.load_dir
dataset = args.dataset
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
torch.backends.cudnn.enabled=False # ???

def Load_data(embed_size):
    if os.path.exists(fname):
        return pickle.load(open(fname,'rb'))
    raise RuntimeError('Data not found.')

def translate(sent, num2word):
    result=[]
    for num in sent:
        word = num2word.get(int(num),'<UNK>')
        result.append(word)
        if word == '<EOS>': break
    return result

def calc_blue(init, predict, label, num2word_cn, num2word_en):
    p = translate(init, num2word_cn)
    x = translate(predict, num2word_en)
    y = translate(label, num2word_en)
    score = sentence_bleu([y],x)
    #'''
    if score>0.5:
        print(p)
        print(x)
        print(y)
        print(score)
        print('----')
        xxx=input('pause')
    #'''
    return score

def reverse(word2num):
    num2word = {}
    for key,val in word2num.items():
        num2word[val]=key
    return num2word

if __name__ == "__main__":
    if dataset=='small':
        fname = './utils/data10000.pkl'
    else:
        fname = './utils/data100000.pkl'
    train_x, train_y, _, _, test_x, test_y, \
    train_dict, test_dict, w2v_train, w2v_test = Load_data(fname)
    num2word_ch = reverse(train_dict)
    num2word_en = reverse(test_dict)
    dim_in = len(train_dict)
    dim_out = len(test_dict)
    train_len = len(train_x)
    test_len = len(test_x)
    print('dataset load done.')
    # load dataset

    model = NMT(dim_in, dim_out,
                ues_attention=attention
                ).to(device)
    if device=='cuda': weight_dict = torch.load(load_dir)
    else: weight_dict = torch.load(load_dir, map_location='cpu')
    model.load_state_dict(weight_dict)
    # load pretrained model
    timer = Timer(test_len/100)
    # get model

    print('testing start.')

    model.eval()
    sum_score = 0
    max_score = 0
    for ind, inputs, labels in zip(range(test_len), test_x, test_y):
        in_data = inputs
        inputs = torch.LongTensor(inputs).to(device).unsqueeze(0)
        outputs = model(inputs, beam_size=5)
        score = calc_blue(in_data, outputs, labels, num2word_ch, num2word_en)
        sum_score += score
        max_score = max(max_score, score)
        if ind%100==0:
            print('data %d, score %f, avg_score %f, max_score %f'%(ind, score, sum_score/(ind+1), max_score))
            timer.update()
        
    avg_score = sum_score / test_len
    print('[avg_score %f]'%(avg_score))
    
    print('Testing end!')

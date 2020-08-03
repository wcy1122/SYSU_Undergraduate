import math
import numpy as np
import torch
import os
import csv
import pickle

def Decay_LR(optimizer, decay_rate):
    old_lr = optimizer.param_groups[0]['lr']
    print("==========<< Change LR from %f to %f >>=========="%(old_lr, old_lr*decay_rate))
    for param_group in optimizer.param_groups:
        param_group['lr'] *= decay_rate

def Split_data(data_x, data_y, data_l, keep_rate):
    data_size = int(len(data_x)*keep_rate)
    train_x = data_x[:data_size]
    train_y = data_y[:data_size]
    train_l = data_l[:data_size]
    val_x = data_x[data_size:]
    val_y = data_y[data_size:]
    val_l = data_l[data_size:]
    return train_x, train_y, train_l, val_x, val_y, val_l

def save_model(base_dir, model, fname):
    file_name = os.path.join('checkpoint', fname)
    out_dir = os.path.join(base_dir, file_name)
    torch.save(model.state_dict(), out_dir)
    print('==========<< save model %s >>=========='%fname)

def save_data(save_dir, num, file_name):
    out_dir = os.path.join(save_dir, 'log')
    out_dir = os.path.join(out_dir, file_name)
    out_file = open(out_dir, "a+")
    print("%f"%num, file = out_file)
    out_file.close()

def init_output_log(save_dir):
    if not os.path.exists(save_dir):
        os.mkdir(save_dir)
    log_dir = os.path.join(save_dir, 'log')
    if not os.path.exists(log_dir):
        os.mkdir(log_dir)
    cp_dir = os.path.join(save_dir, 'checkpoint')
    if not os.path.exists(cp_dir):
        os.mkdir(cp_dir)
    file_list = [
        'train_loss.txt', 
		'valid_loss.txt',
        'train_acc.txt', 
		'valid_acc.txt'
    ]
    for file_name in file_list:
        file_name = os.path.join(log_dir, file_name)
        out_file = open(file_name, "w")
        out_file.close()

def write_csv(out_dir, predict):
    out_dir = os.path.join(out_dir, '%s_result.csv'%(out_dir))
    print(out_dir)
    out_file = open(out_dir, 'w', encoding='utf-8')
    csv_writer = csv.writer(out_file)
    csv_writer.writerow(['PhraseId','Sentiment'])
    for i in range(len(predict)):
        idx = 156061 + i
        csv_writer.writerow([idx, int(predict[i].data)])
    out_file.close()

def load_embadding(embd_fname, embd_size, _dict):
    embd_pkl_dir = os.path.join('data','pretrain','embadding_%d.pkl'%embd_size)
    print("Load pretrain embadding weight from", embd_fname)
    if os.path.exists(embd_pkl_dir):
        return pickle.load(open(embd_pkl_dir, 'rb'))
    
    embd_txt_dir = os.path.join('data','pretrain',embd_fname)
    glove_file = open(embd_txt_dir, "r")
    embadding = np.random.normal(scale=0.6, size=(len(_dict), embd_size))
    Cnt = 0
    for line in glove_file.readlines():
        line = line.split(' ')
        key = line[0]
        value = np.array(line[1:]).astype(np.float)
        if _dict.get(key, None):
            embadding[_dict[key]] = value
            Cnt += 1
    
    pickle.dump(embadding, open(embd_pkl_dir, 'wb'))
    return embadding

def get_samples_weight(train_y):
    class_sample_count = np.array([len(np.where(train_y == t)[0]) for t in range(5)])
    print(class_sample_count/len(train_y))
    weight = 1. / class_sample_count
    samples_weight = np.array([weight[t] for t in train_y])
    samples_weight = torch.from_numpy(samples_weight)
    return samples_weight

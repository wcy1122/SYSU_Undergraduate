import time
import numpy as np
from collections import defaultdict
import os

class Timer:
    def __init__(self, epoch_size):
        self.epoch_size = epoch_size
        self.start_time = time.time()
        self.pre_time = time.time()
        self.epoch = 0
    
    def update(self):
        self.epoch += 1
        now_time = time.time()
        tot_time = now_time - self.start_time
        expext_time = tot_time * self.epoch_size / self.epoch
        remain_time = expext_time - tot_time
        epoch_time = now_time - self.pre_time
        self.pre_time = now_time
        print("epoch_time: %02d:%02d:%02d, expect_time: %02d:%02d:%02d, remain_time: %02d:%02d:%02d"%(
                epoch_time/3600, (epoch_time%3600)/60, epoch_time%60,
                expext_time/3600, (expext_time%3600)/60, expext_time%60,
                remain_time/3600, (remain_time%3600)/60, remain_time%60
            )
        )

def batch_iter(data, step, batch_size):
    return data[step*batch_size : min((step+1)*batch_size, len(data))]

save_dir = None
def get_dir(output_dir, file_name):
    return os.path.join(output_dir, file_name)

def save_data(num, file_name):
    global save_dir
    out_dir = get_dir(save_dir, 'log')
    out_dir = get_dir(out_dir, file_name)
    out_file = open(out_dir, "a+")
    print("%f"%num, file = out_file)
    out_file.close()

def init_output_log(direct):
    global save_dir
    save_dir = direct
    if not os.path.isdir(save_dir):
        os.mkdir(save_dir)
    log_dir = os.path.join(save_dir, 'log')
    if not os.path.isdir(log_dir):
        os.mkdir(log_dir)
    cp_dir = os.path.join(save_dir, 'checkpoint')
    if not os.path.isdir(cp_dir):
        os.mkdir(cp_dir)
    file_list = [
        'train_loss.txt', 
		'valid_loss.txt'
    ]
    for file_name in file_list:
        file_name = get_dir(log_dir, file_name)
        out_file = open(file_name, "w")
        out_file.close()
# File Saver
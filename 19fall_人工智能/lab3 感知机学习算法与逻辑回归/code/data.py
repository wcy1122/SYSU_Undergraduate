import csv
import os
import random
import numpy as np

def get_data(path, shuffle=False):
    input_file = open(path,'r')
    reader = csv.reader(input_file)
    data_x=[]; data_y=[]
    for line in reader:
        data_x.append(line[:40])
        data_y.append(line[-1])
    data_x=np.array(data_x).astype(float)
    data_y=np.array(data_y).astype(float)

    if shuffle==True:
        state=np.random.get_state()
        np.random.shuffle(data_x)
        np.random.set_state(state)
        np.random.shuffle(data_y)

    return data_x, data_y
# 数据获取

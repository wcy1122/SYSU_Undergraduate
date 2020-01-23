import csv
import os
import random
import numpy as np
from math import sqrt,log2,exp
from numpy.linalg import svd,norm
import math
import matplotlib.pyplot as plt
from operator import eq

train_path="./car_train.csv"
cnt_node=0

def get_train_data(path, shuffle=False):
    input_file = open(path,'r')
    reader = csv.reader(input_file)
    index = -1
    data_x=[]; data_y=[]; attribute=[]
    for line in reader:
        if index<0: attribute=line[:6];index=0;continue
        feature = line[:6]
        lable = line[6]
        feautre_dict={}
        for x,y in zip(attribute,feature):
            feautre_dict[x]=y
        data_x.append(feautre_dict)
        data_y.append(lable)
    data_x = np.array(data_x)
    data_y = np.array(data_y)

    if shuffle==True:
        state=np.random.get_state()
        np.random.shuffle(data_x)
        np.random.set_state(state)
        np.random.shuffle(data_y)

    return data_x,data_y,attribute
# 数据获取

def split_data(data_x,data_y,K):
    size = len(data_x)
    block = (size+K-1)//K
    res_x = []; res_y = []
    for i in range(K):
        pos = list(range(i*block,min((i+1)*block,size)))
        res_x.append(data_x[pos])
        res_y.append(data_y[pos])
    res_x = np.array(res_x)
    res_y = np.array(res_y)
    return res_x,res_y
# K折交叉验证数据集分割

def calc(data,method='entropy'):
    len_data = len(data)
    label_list = set()
    if method=='gini': result = 1
    else : result=0
    for index in data: 
        label_list.add(index)
    for index in label_list:
        size = len([y for y in data if y==index])
        P = size/len_data
        if method=='entropy': result -= P*log2(P)
        else : result -= P*P
    return result
# 信息熵

def calc_splitinfo(data):
    entropy = 0; cnt_D = 0
    for index in data: 
        cnt_D += len(index)
    for index in data:
        P = len(index)/cnt_D
        entropy -= P*log2(P)
    if entropy==0: return 1e-6
    return entropy
# 属性混乱度

def split_set(train_x, train_y, feature):
    feature_list = set(); res_x=[]; res_y=[]
    for index in train_x:
        feature_list.add(index[feature])
    feature_list = list(feature_list)
    for index in feature_list:
        pos = [i for i in range(len(train_x)) if train_x[i][feature] == index]
        res_x.append(train_x[pos])
        res_y.append(train_y[pos])
    res_x = np.array(res_x)
    res_y = np.array(res_y)
    return res_x, res_y, feature_list
# 根据label划分子树

def get_label(data):
    max_size = 0; label = ''; label_list = set()
    for index in data: 
        label_list.add(index)
    for index in label_list:
        size = len([y for y in data if y==index])
        if size > max_size: max_size = size;label = index
    return label
# 获取节点的label

def equal(data):
    tmp=data[0]
    for item in data:
        if not tmp==item: return False
    return True
# 判断是否一叉树

def testing(now,train_x,tree):
    _, child, index, feature_set, label = tree[now-1]
    if child==[]: return label
    for ch, item in zip(child, feature_set):
        if train_x[index]==item:
            return testing(ch, train_x, tree)
    return label
# 测试


def get_accuracy(lab1,lab2):
    result=0
    for x,y in zip(lab1,lab2):
        if x==y: result+=1
    return 1.0*result/len(lab1)
# 评估

def training(now, train_x, train_y, valid_x, valid_y, 
             attribute, method='ID3', val='yes', C=2.5, depth=10000 ):
    global cnt_node
    H=calc(train_y,method='entropy')
    label=get_label(train_y)
    wrong_train = len([x for x in train_y if x!=label])
    wrong_valid = len([x for x in valid_y if x!=label])

    if len(attribute)==0 or H==0 or equal(train_x) or depth<=0:
        return [(now,[],0,0,label)], wrong_train, wrong_valid, 1
    # 终止条件

    len_data = len(train_x)
    feature=''; max_h=-10000
    # 初始化

    for item in attribute:
        now_h=0
        new_x,new_y,_ = split_set(train_x, train_y, item)
        for now_x,now_y in zip(new_x,new_y):
            P=len(now_x)/len_data
            if method=='CART': now_h += P*calc(now_y,method='gini')
            else: now_h += P*calc(now_y,method='entropy')
            # 计算条件信息熵 or Gini指数
        if method=='ID3': now_h=H-now_h # 信息增益
        if method=='C4.5': now_h = (H-now_h)/calc_splitinfo(new_x) # 信息增益率
        if method=='CART': now_h = -now_h # gini最小 -> -gini最大
        if now_h>max_h: max_h=now_h; feature=item
    # 寻找最优特征

    tree=[]; child_now=[]; train_split=0; valid_split=0; leaves_size=0
    attribute.remove(feature)
    tri_x, tri_y, feature_set = split_set(train_x, train_y, feature)
    # 分割数据集
    
    for i in range(len(tri_x)):
        cnt_node+=1; val_x=[]; val_y=[]
        child_now.append(cnt_node)
        if val=='yes':
            for x,y in zip(valid_x, valid_y):
                if x[feature]==feature_set[i]:
                    val_x.append(x); val_y.append(y)
        tree_son, train_son, valid_son, leaves = training(cnt_node, tri_x[i], tri_y[i], val_x, val_y, 
                                                 attribute.copy(), method=method, val=val, C=C, depth=depth-1 )
        train_split += train_son
        valid_split += valid_son
        leaves_size += leaves
        tree.extend(tree_son)
    # 递归

    node = (now,child_now,feature,feature_set,label)

    if val=='yes':
        if valid_split > wrong_valid:
            node = (now,[],0,0,label); leaves_size=1
            train_split = wrong_train
        else: wrong_valid = valid_split
    # 后剪枝
    if train_split+C*leaves_size > wrong_train + C:
        node = (now,[],0,0,label); leaves_size=1
    else: wrong_train = train_split
    # 泛化错误率
    
    tree.append(node)
    return tree, wrong_train, wrong_valid, leaves_size
# 训练，深搜建树

def trainsform(trains,filter_list):
    res=[]
    for i in filter_list:
        res.extend(trains[i])
    return np.array(res)
# 数据格式转换


def split_train_val(trains_x, trains_y):
    pos = int(len(trains_x)*9/10)
    train_x, train_y = trains_x[:pos], trains_y[:pos]
    valid_x, valid_y = trains_x[pos:], trains_y[pos:]
    return train_x, train_y, valid_x, valid_y


def solve(train_x, train_y, attribute, K=5, method='ID3', val='no', C=0, max_depth=10000):
    assert(method == 'ID3' or method == 'C4.5' or method == 'CART')
    assert(val == 'no' or val == 'yes')
    train_x, train_y = split_data(train_x, train_y, K)
    sum_accuracy = 0

    global cnt_node
    for i in range(K):
        filter_list = [j for j in range(K) if i != j]
        trains_x = trainsform(train_x, filter_list)
        trains_y = trainsform(train_y, filter_list)

        valids_x=[]; valids_y=[]
        if val=='yes':
            trains_x, trains_y, valids_x, valids_y = split_train_val(trains_x, trains_y)
        # 分割验证集
        
        cnt_node=1
        tree,train_accuracy, val_accuracy,_ = training(1, trains_x, trains_y, valids_x, valids_y, 
                                              attribute.copy(), method=method, val=val, C=C, depth=max_depth )
        train_accuracy = 1-train_accuracy/len(trains_x)
        if val=='yes': val_accuracy = 1-val_accuracy/len(valids_x)
        # 训练
        
        tree.sort(); predict=[]
        for x in train_x[i]:
            predict.append(testing(1, x, tree))
        test_accuracy=get_accuracy(predict,train_y[i])
        sum_accuracy+=test_accuracy
        # 评估

        print("train_accuracy= %f"%train_accuracy)
        if val=='yes': print("val_accuracy= %f"%val_accuracy)
        print("test_accuracy= %f"%test_accuracy)
    
    avg_accuracy=sum_accuracy/K
    print("k= %d, method=%s, val=%s, C=%f, max_depth=%d, average accuracy= %f"%(K, method, val, C, max_depth, avg_accuracy))
    return avg_accuracy
# 训练，K折交叉验证

def plot_result(X,Y,label):
    plt.plot(X,Y,label=label)
# 画图

train_x, train_y, attribute = get_train_data(train_path, shuffle=False)
C_list = np.linspace(0,0.5,num=6,endpoint=True)
D_list = np.linspace(0,6,num=7,endpoint=True)
val_list=['no','yes']
method_list=['ID3','C4.5','CART']

avg_accuracy=solve(train_x, train_y, attribute, K=5, method='CART', val='yes', C=0, max_depth=10000)

import os
import numpy as np
from data import get_data
from models import perceptron as PLA
from models import logistics_regression as LR
import matplotlib.pyplot as plt
'''
from sklearn.linear_model import Perceptron as sklearn_PLA
from sklearn.linear_model import LogisticRegression as sklearn_LR
from sklearn.svm import SVC
'''

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

def normalize(data):
    for i in range(40):
        std = np.std(data[:,i])
        mean = np.mean(data[:,i])
        data[:,i] -= mean
        data[:,i] /= std
    return data

def trainsform(trains, filter_list):
    res=[]
    for i in filter_list:
        res.extend(trains[i])
    return np.array(res)
# 数据格式转换

def plot_result(X,Y,label):
    plt.plot(X,Y,label=label)

def solve(train_x, train_y, test_x, test_y , K=5, method='PLA', iter_method='whole', 
          optimizer='BGD', dynamic_lr=False, batch_size=9999):

    train_x, train_y = split_data(train_x, train_y, K)
    assert(method=='PLA' or method=='LR')

    #mean_test_accuracy = 0
    #mean_train_accuracy = 0
    for i in range(K):
        #filter_list = [j for j in range(K) if i != j]
        #trains_x = trainsform(train_x, filter_list)

        #trains_y = trainsform(train_y, filter_list)
        #ests_x = train_x[i]
        #tests_y = train_y[i]
            
        trains_x = train_x[0]
        trains_y = train_y[0]

        if method=='PLA': 
            model = PLA(num_class=40, lr=1, iter_method=iter_method)
            model.fit(trains_x, trains_y, iter_size=100)
        elif method=='LR': 
            model = LR(num_class=40, lr=1, optimizer=optimizer)
            model.fit(trains_x, trains_y, iter_size=100, batch_size=batch_size, dynamic_lr=dynamic_lr)
            
        #pred = model.predict(tests_x)
        #test_accuracy = model.score(tests_y,pred)
        #mean_test_accuracy += test_accuracy

        pred = model.predict(trains_x)
        train_accuracy = model.score(trains_y, pred)
        #mean_train_accuracy += train_accuracy

        #print("fold %d, test accuracy %f"%(i, test_accuracy))
        print("fold %d, train accuracy %f"%(i, train_accuracy))

        pred = model.predict(test_x)
        
        
        test_accuracy = model.score(test_y,pred)
        print(pred)
        print(test_y)
        print("fold %d, test accuracy %f"%(i, test_accuracy))
        #test_accuracy = model.score(tests_y,pred)

        
    #ean_test_accuracy/=K
    #mean_train_accuracy/=K
        
    #print("mean_test_accuracy %f"%mean_test_accuracy)
    #print("mean_train_accuracy %f"%mean_train_accuracy)


train_path="./check_train.csv"
test_path="./check_test.csv"
train_x, train_y = get_data(train_path, shuffle=False)
test_x, test_y = get_data(test_path, shuffle=False)
#train_x = normalize(train_x)
#solve(train_x, train_y, test_x, test_y , K=1, method='LR', optimizer='BGD', dynamic_lr=False)
#os.system("pause")
solve(train_x, train_y, test_x, test_y , K=1, method='PLA', iter_method='first')

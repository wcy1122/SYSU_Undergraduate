import csv
import os
import numpy as np
from math import sqrt,log10,exp
from numpy.linalg import svd,norm
import math
import matplotlib.pyplot as plt

path="./lab1_data/classification_dataset"
train_path=path+'/train_set.csv'
valid_path=path+'/validation_set.csv'
test_path=path+'/test_set.csv'
output_path="./Output/classify/"

def get_train_data(path):
    text_lable=[]
    num_word=[]
    word_list={}
    word_pos={}
    with open(path,'r') as input_file:
        reader=csv.reader(input_file)
        
        index=-1
        for line in reader:
            if index<0: index=0;continue
            text,lable=line
            text_lable.append(lable)
            text=text.split(' ')
            num_word.append(len(text))
            for word in text:
                if not word_pos.get(word):
                    word_pos[word]=len(word_pos)
                word_list.setdefault(word,[]).append(index)
            index+=1

    text_size=len(num_word)
    word_size=len(word_list)
    OneHot=np.zeros([text_size,word_size])
    TFIDF=np.zeros([text_size,word_size])
    IDF=np.zeros([word_size])

    for (key,values) in word_list.items():
        i=word_pos[key]
        length=len(values);pre=''
        for j in values:
            if j==pre: length-=1
            pre=j
        IDF[i]=log10(text_size/(1+length))
        for j in values: OneHot[j][i]+=1
    
    for i in range(text_size):
        for j in range(word_size):
            TF=OneHot[i][j]/num_word[i]
            TFIDF[i][j]=TF*IDF[j]
    
    return word_list,word_pos,OneHot,TFIDF,text_lable


def get_valid_data(word_list,word_pos,path):
    text_lable=[]
    num_word=[]
    word_size=len(word_list)
    word_cnt=np.zeros([word_size])

    with open(path,'r') as input_file:
        reader=csv.reader(input_file)
        OneHot=[]
        
        index=-1
        for line in reader:
            if index<0: index=0;continue
            text,lable=line
            text_lable.append(lable)
            text=text.split(' ')
            num_word.append(len(text))
            feature=np.zeros([word_size]); tag={}
            for word in text:
                if word_pos.get(word):
                    feature[word_pos[word]]+=1
                    if not tag.get(word):
                        word_cnt[word_pos[word]]+=1; tag[word]=1
            OneHot.append(feature)
    
    OneHot=np.array(OneHot)
    text_size=len(num_word)
    IDF=np.zeros([word_size])
    TFIDF=np.zeros([text_size,word_size])
    
    for key in word_list.keys():
        i=word_pos[key]
        IDF[i]=log10(text_size/(1+word_cnt[i]))
    
    for i in range(text_size):
        for j in range(word_size):
            TF=OneHot[i][j]/num_word[i]
            TFIDF[i][j]=TF*IDF[j]

    return OneHot,TFIDF,text_lable

def get_test_data(word_list,word_pos,path):
    num_word=[]
    word_size=len(word_list)
    word_cnt=np.zeros([word_size])

    with open(path,'r') as input_file:
        reader=csv.reader(input_file)
        OneHot=[]
        
        index=-1
        for line in reader:
            if index<0: index=0;continue
            text=line[1].split(' ')
            num_word.append(len(text))
            feature=np.zeros([word_size]); tag={}
            for word in text:
                if word_pos.get(word):
                    feature[word_pos[word]]+=1
                    if not tag.get(word):
                        word_cnt[word_pos[word]]+=1; tag[word]=1
            OneHot.append(feature)
    
    OneHot=np.array(OneHot)
    text_size=len(num_word)
    IDF=np.zeros([word_size])
    TFIDF=np.zeros([text_size,word_size])
    for key in word_list.keys():
        i=word_pos[key]
        IDF[i]=log10(text_size/(1+word_cnt[i]))
    
    for i in range(text_size):
        for j in range(word_size):
            TF=OneHot[i][j]/num_word[i]
            TFIDF[i][j]=TF*IDF[j]

    return OneHot,TFIDF


def get_distance(x,y,type='cos'):
    if type=='L1':
        return norm(x-y, ord=1)
    elif type=='L2':
        return norm(x-y, ord=2)
    elif type=='cos':
        A=norm(x); B=norm(y)
        if A==0 or B==0: return 1e9
        return 1-np.dot(x,y)/(A*B)
    else: assert(0)

def get_accuracy(pred,label):
    accuracy=0
    for x,y in zip(pred,label):
        if x==y: accuracy+=1
    return accuracy/len(pred)


def gaussian(dist, a=1, b=0, c=0.3):
    return a * math.e ** (-(dist - b) ** 2 / (2 * c ** 2))

def testing(trains_x, trains_y, tests_x, K=13, distance='cos', weight=1):
    prediction=[]
    for test_x in tests_x:
        val=[]
        for train_x,train_y in zip(trains_x,trains_y):
            dis=get_distance(train_x,test_x,type=distance)
            val.append((dis,train_y))
        val.sort()
        # get distance

        dict_feature={}
        for res in val[:K]:
            dis,word=res
            if not dict_feature.get(word): dict_feature[word]=0
            if weight==0: dict_feature[word]+=1
            else: dict_feature[word]+=gaussian(dis)
        # calculate k nearest distance
        
        maxn=-1;predict=''
        for key,val in dict_feature.items():
            if val>maxn: predict=key;maxn=val
        prediction.append(predict)
        # prediction
    
    return prediction


def write_result(result):
    in_file=open(test_path,'r').readlines()
    for i in range(len(in_file)):
        if i==0: continue
        line=in_file[i].split(',')
        line[2]=str(result[i-1])
        in_file[i]=','.join(line)
    if not os.path.exists(output_path):
        os.makedirs(output_path)
    output_file=output_path+"17341146_WangChengyao_KNN_classification.csv"
    open(output_file,'w').write('\n'.join(in_file))

def plot_result(X,Y,label):
    plt.plot(X,Y,label=label)
    

word_list, word_pos, tr_onehot, trains_x, trains_y= get_train_data (train_path)
# get training set
va_onehot, valids_x, valids_y = get_valid_data (word_list, word_pos, valid_path)
# get valiation set
te_onehot, tests_x = get_test_data (word_list, word_pos, test_path)
# get test set

def solve(trains_x, trains_y, valids_x, tests_x, K_range=range(1,31), distance='cos', weight=1, feature='TF-IDF'):
    max_A=0; max_K=0; X=[]; Y=[]
    for K in K_range:
        predict=testing(trains_x, trains_y, valids_x, K, distance, weight)
        print(len(valids_x))
        accuracy=get_accuracy(predict,valids_y)
        print("K = %d, accuracy = %f"%(K,accuracy))
        if accuracy>max_A: max_A=accuracy; max_K=K
        X.append(K)
        Y.append(accuracy)
        print(predict)
    print("max_K = %d, max_accuracy = %f"%(max_K,max_A))
    plot_result(X,Y,'KNN classify accuracy')
    
    result=testing(trains_x, trains_y, tests_x, max_K)
    write_result(result)


plt.title('KNN classify result')
plt.xlabel('K')
plt.ylabel('Accuracy')
solve(trains_x, trains_y, valids_x, tests_x)
plt.legend()
plt.show()




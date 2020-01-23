import re
import os
import numpy as np
from math import log10
input_path="./lab1_data/semeval.txt"
output_path="./Output/TF-IDF"
file_name="/17341146_WangChengyao_TFIDF.txt"
word_list={}
word_pos={}
num_word=[]

index=0
with open(input_path,'r') as in_file:
    index=0
    for line in in_file:
        num,label,text,_=re.split('\t|\n',line)
        text=text.split(' ')
        num_word.append(len(text))

        for word in text:
            if not word_pos.get(word):
                word_pos[word]=len(word_pos)
                # map word to integer
            word_list.setdefault(word,[]).append(index)
            # add word to dictionary
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
        # deduct repetitive word
    IDF[i]=log10(text_size/(1+length))
    for j in values: OneHot[j][i]+=1

for i in range(text_size):
    for j in range(word_size):
        TF=OneHot[i][j]/num_word[i]
        TFIDF[i][j]=TF*IDF[j]

if not os.path.exists(output_path):
    os.makedirs(output_path)
with open(output_path+file_name,'w') as out_file:
    for i in range(text_size):
        for j in range(word_size):
            print(" %f"%TFIDF[i][j],end="",file=out_file)
        print("",file=out_file)
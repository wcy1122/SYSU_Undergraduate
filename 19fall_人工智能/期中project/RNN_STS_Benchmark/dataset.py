import csv
import nltk
import re
import tensorflow as tf
import pickle
import torch
import numpy as np
from nltk.corpus import stopwords 
from nltk.tokenize import word_tokenize 
from nltk.stem import SnowballStemmer
from gensim.models import Word2Vec
from gensim.models.word2vec import LineSentence


file_list = ['./lib/sts-train.csv','./lib/sts-dev.csv','./lib/sts-test.csv']
train_path = './lib/sts-train.csv'
test_path = './lib/sts-test.csv'

def load_data(path):
	data = []
	match_rate = []
	with open(path,encoding='utf-8') as f:
		f_csv = csv.reader(f)
		for line in f_csv:
			row = ', '.join(line)
			row = row.split('\t')
			match_rate.append(float(row[4]))
			data.append(simplify(row[5]))
			data.append(simplify(row[6]))

	return data,match_rate

def simplify(sentence):
	# 取消缩写
    sentence = sentence.lower().replace("it's", "it is").replace("i'm", "i am").replace("he's", "he is").replace("she's", "she is")\
    .replace("we're", "we are").replace("they're", "they are").replace("you're", "you are").replace("that's", "that is")\
    .replace("this's", "this is").replace("can't", "can not").replace("don't", "do not").replace("doesn't", "does not")\
    .replace("we've", "we have").replace("i've", " i have").replace("isn't", "is not").replace("won't", "will not")\
    .replace("hasn't", "has not").replace("wasn't", "was not").replace("weren't", "were not").replace("let's", "let us")
    #删除标点
    sentence = re.sub("[^a-zA-Z]", " ", sentence)
    my_words = sentence.split()
    tmp_words = []
    # 删除停止词 还原语态
    stemmer = SnowballStemmer("english")
    sw = stopwords.words('english')
    for word in my_words:
    	if word not in sw:
    		tmp_words.append(stemmer.stem(word))
    return ' '.join(tmp_words)

def embdding(data,w_dict):
	data_mat1 = []
	data_mat2 = []
	for i in range(len(data)):
		seq = []
		words = data[i].split()
		for word in words:
			seq.append(list(w_dict[word]))
		if i < 6000:
			data_mat1.append(seq)
		else:
			data_mat2.append(seq)
	# print(data_mat[0])
	pad_sequence = tf.keras.preprocessing.sequence.pad_sequences
	data_mat1 = pad_sequence(data_mat1,maxlen = 15,dtype = 'float64',padding = 'post',truncating = 'post',value = 0)
	data_mat2 = pad_sequence(data_mat2,maxlen = 15,dtype = 'float64',padding = 'post',truncating = 'post',value = 0)
	data_mat1 = torch.Tensor(data_mat1)
	data_mat2 = torch.Tensor(data_mat2)

	if len(data_mat2) > 0:
		data_mat = torch.cat((data_mat1,data_mat2),0)
	else:
		return torch.Tensor(data_mat1)
	print(data_mat.shape)
	return data_mat


def train_word2vec():
	train_w2v = []
	for path in  file_list:
		print('Reading',path)
		with open(path,encoding='utf-8') as f:
			f_csv = csv.reader(f)
			for line in f_csv:
				row = ', '.join(line)
				row = row.split('\t')
				train_w2v.append(simplify(row[5]))
				train_w2v.append(simplify(row[6]))
	with open('./lib/w2v.txt','w',encoding = 'utf-8') as ts:
		for line in train_w2v:
			ts.write(line+'\n')
	sentence_data = './lib/w2v.txt'
	print('Training')
	model = Word2Vec(LineSentence(sentence_data),sg = 0,size = 50,min_count=0,workers = 20)
	model.save('word2vec')
	print('Training finished!')

def data_save(data,file):
	pickle.dump(data, open('./lib/'+file,'wb'))

# def get_data():
if __name__=="__main__":
	train_data,train_rate = load_data(train_path)
	test_data,test_rate = load_data(test_path)
	print('load data done.')
	#train_word2vec()
	model = Word2Vec.load('word2vec')
	# 训练 测试数据
	train_mat = embdding(train_data,model)
	test_mat = embdding(test_data,model)
	
	data_save(train_mat,'trainD.pkl')
	data_save(train_rate,'trainR.pkl')
	data_save(test_mat,'testD.pkl')
	data_save(test_rate,'testR.pkl')
	# print(data_mat[2:4])
	# return data_mat,std_rate
	
import torch
import os
import pickle
import re

def load_data(in_dir, _type='train', _sent_only=False):
    in_file = open(in_dir, "r")
    _first = True
    data_x = []
    data_y = []

    _pre = 0
    for line in in_file.readlines():
        if _first==True: 
            _first = False; continue
        if _type=='train':
            _,_num,x,y = line.rstrip("\n").split('\t')
        else:
            _,_num,x = line.rstrip("\n").split('\t')
        
        _num = int(_num)
        if _sent_only==False or _num != _pre:
            data_x.append(x)
            if _type=='train':
                data_y.append(int(y))
        _pre = _num

    if _type=='test': return data_x
    else: return data_x, data_y

def simplify(text):
    text = text.lower().replace("it's", "it is").replace("i'm", "i am")\
            .replace("he's", "he is").replace("she's", "she is").replace("we're", "we are")\
            .replace("they're", "they are").replace("you're", "you are").replace("that's", "that is")\
            .replace("this's", "this is").replace("can't", "can not").replace("don't", "do not")\
            .replace("doesn't", "does not").replace("we've", "we have").replace("i've", " i have")\
            .replace("isn't", "is not").replace("won't", "will not").replace("hasn't", "has not")\
            .replace("wasn't", "was not").replace("weren't", "were not").replace("let's", "let us")
    text = re.sub("[^a-zA-Z0-9]", " ", text)
    return text

def build_dict(train_x, our_dir):
    dict_dir = os.path.join(our_dir,'dict.pkl')
    if os.path.exists(dict_dir):
        word_dict = pickle.load(open(dict_dir,'rb'))
        return word_dict
    word_dict = {'<PAD>':0, '<UNK>':1}
    sum_index = 2
    max_len = 0
    for _line in train_x:
        _line = simplify(_line)
        line = _line.split(' ')
        max_len = max(max_len, len(line))
        for word in line:
            if word not in word_dict:
                word_dict[word] = sum_index
                sum_index += 1
    print("Build dictionary with size", sum_index)
    pickle.dump(word_dict, open(dict_dir,'wb'))
    return word_dict

def load_dict(in_dir):
    dict_dir = os.path.join(in_dir,'dict.pkl')
    assert os.path.exists(dict_dir)
    return pickle.load(open(dict_dir,'rb'))

def word2index(train_x, _dict):
    index_list = []
    for line in train_x:
        indexs = [_dict.get(word, 1) for word in line.split(' ')]
        index_list.append(indexs)
    return index_list

def Padding(train_x):
    lengths = [len(x) for x in train_x]
    max_len = max(lengths)
    train_new_x = []
    for line in train_x:
        line.extend([0] * (max_len - len(line)))
        train_new_x.append(line)
    train_x = torch.LongTensor(train_new_x)
    lengths = torch.LongTensor(lengths)
    return train_x, lengths

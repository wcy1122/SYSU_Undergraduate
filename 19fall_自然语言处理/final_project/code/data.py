import os
import re
import pickle
import jieba
import numpy as np
from nltk.tokenize import word_tokenize
from gensim.models import Word2Vec
import warnings
warnings.filterwarnings(action='ignore', category=UserWarning, module='gensim')

def load_stopword(path):
    in_file = open(path, 'r', encoding='utf-8')
    stopwords = in_file.readlines()
    stopwords = [x.rstrip("\n") for x in stopwords]
    return stopwords

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

def split_word(path, language, stopwords):
    all_text = []
    if language == 'Chinese':
        remove_index = []

    in_file = open(path, 'r', encoding='utf-8')
    texts = in_file.readlines()

    for i in range(len(texts)):
        text = texts[i].rstrip("\n")

        if language == 'Chinese':
            text = list(jieba.cut(text, cut_all=False))
            text = [x for x in text if x not in stopwords] 
        else:
            text = simplify(text)
            text = word_tokenize(text)
        
        text.insert(0, '<BOS>')
        text.append('<EOS>')
        all_text.append(text)

    return all_text

def build_dic(texts, tp):
    fname = ('./utils/%s_dict.pkl'%tp)
    if os.path.exists(fname):
        return pickle.load(open(fname,'rb'))
    INDEX = 4
    word_dict = {'<BOS>':0, '<EOS>':1, '<PAD>':2, '<UNK>':3}
    for text in texts:
        for word in text:
            if word not in word_dict:
                word_dict[word] = INDEX; INDEX+=1
    pickle.dump(word_dict, open(fname,'wb'))
    return word_dict

def train_w2v(text, embed_size, word_dict, tp):
    fname = ('./utils/w2v%s.pkl'%tp)
    if os.path.exists(fname):
        return pickle.load(open(fname,'rb'))
    else:
        model = Word2Vec(text, size=embed_size, window=5, min_count=1, workers=20)
        num2vec = np.zeros((len(word_dict), embed_size))
        for key,val in word_dict.items():
            try: num2vec[val] = model[key]
            except: pass
        num2vec = np.array(num2vec)
        pickle.dump(num2vec, open(fname,'wb'))
        return num2vec

def translate(texts, word_dict):
    data = []
    UNK = word_dict['<UNK>']
    for text in texts:
        seq = []
        for word in text:
            seq.append(word_dict.get(word, UNK))
        data.append(seq)
    return data

if __name__ == "__main__":
    fname = './utils/data100000.pkl'
    embed_size = 128
    
    train_source = './dataset/dataset_100000/train_source_80000.txt'
    train_target = './dataset/dataset_100000/train_target_80000.txt'
    dev_source = './dataset/dataset_100000/dev_source_10000.txt'
    dev_target = './dataset/dataset_100000/dev_target_10000.txt'
    test_source = './dataset/dataset_100000/test_source_10000.txt'
    test_target = './dataset/dataset_100000/test_target_10000.txt'
    stop_words = './utils/stopwords.txt'
    
    stopwords = load_stopword(stop_words)

    train_x = split_word(train_source, 'Chinese', stopwords)
    train_y = split_word(train_target, 'English', stopwords)
    dev_x = split_word(dev_source, 'Chinese', stopwords)
    dev_y = split_word(dev_target, 'English', stopwords)
    test_x = split_word(test_source, 'Chinese', stopwords)
    test_y = split_word(test_target, 'English', stopwords)

    train_dict = build_dic(train_x, 'train')
    test_dict = build_dic(train_y, 'test')
    w2v_train = train_w2v(train_x, embed_size, train_dict, 'train')
    w2v_test = train_w2v(test_x, embed_size, test_dict, 'test')

    train_x = translate(train_x, word_dict=train_dict)
    train_y = translate(train_y, word_dict=test_dict)
    dev_x = translate(dev_x, word_dict=train_dict)
    dev_y = translate(dev_y, word_dict=test_dict)
    test_x = translate(test_x, word_dict=train_dict)
    test_y = translate(test_y, word_dict=test_dict)

    pickle.dump((train_x, train_y, dev_x, dev_y, test_x, test_y, 
                train_dict, test_dict, w2v_train, w2v_test), open(fname,'wb'))



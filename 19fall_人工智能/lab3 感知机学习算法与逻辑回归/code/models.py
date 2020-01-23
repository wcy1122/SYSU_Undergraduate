import os
import numpy as np
import matplotlib.pyplot as plt



class perceptron():
    def __init__(self, num_class, lr=1, no_bias=False, iter_method='whole'):
        self.num_class=num_class
        self.lr=lr
        self.weight=np.zeros(num_class)
        self.saved_weight=np.zeros(num_class)
        self.bias=0
        self.saved_bias=0
        self.no_bias=no_bias
        self.pos=0
        self.iter_method=iter_method
    
    def set_lr(self, lr):
        self.lr=lr
    
    def reset_weight(self):
        self.weight=np.zeros(self.num_class)
        self.saved_weight=np.zeros(self.num_class)
        self.bias=0
        self.saved_bias=0
        self.pos=0
    
    def save_weight(self):
        self.saved_weight = self.weight.copy()
        if not self.no_bias: self.saved_bias = self.bias

    def calc(self, x, type='save'):
        if type=='current':
            result=np.dot(x, self.weight)
            if not self.no_bias: result+=self.bias
        elif type=='save':
            result=np.dot(x, self.saved_weight)
            if not self.no_bias: result+=self.saved_bias
        else: assert(0)
        return np.sign(result)

    def predict(self, test_x, type='save'):
        return self.calc(test_x, type=type)
    # 预测

    def score(self, lab1, lab2):
        result=0
        for x,y in zip(lab1,lab2):
            if y==-1: y=0
            if x==y: result+=1
        return 1.0*result/len(lab1)
    # 准确率评估

    def fit(self, train_x, train_y, iter_size=50000):
        length = len(train_x)
        self.reset_weight()
        #max_accuracy = 0
        for i in range(iter_size):
            cnt_correct = 0
            if self.iter_method=='first': self.pos=0
            while cnt_correct<length:
                x = train_x[self.pos]
                y = train_y[self.pos]
                self.pos = (self.pos+1)%length
                if y==0: y=-1
                if self.calc(x, type='current')!=y:
                    self.weight += self.lr*y*x
                    self.bias += self.lr*y
                    print(self.weight)
                    os.system("pause")
                    break
                else: cnt_correct+=1
            '''
            if i%10==0:
                pred = self.predict(train_x,type='current')
                accuracy = self.score(train_y, pred)
                if accuracy> max_accuracy:
                    max_accuracy = accuracy
                    self.save_weight()
                if i%5000==0: 
                    print("iter %d, cur_accuracy %f, max_accuracy %f"%(i, accuracy, max_accuracy))
            '''

            print(self.weight)
            print(self.bias)
            print("epoch %d ---------------"%i)
        self.save_weight()
        #print("optimize done, max_train_accuracy %f"%max_accuracy)
    #训练
    

class logistics_regression():
    def __init__(self, num_class, lr=1, optimizer='Adam', no_bias=False, beta1=0.9, beta2=0.999, epislon=1e-8):
        assert(optimizer=='BGD' or optimizer=='MBGD' or optimizer=='Adam')
        
        self.num_class=num_class
        self.lr=lr
        self.weight=np.zeros(num_class)
        self.bias=0
        self.no_bias=no_bias
        self.optimizer=optimizer
        
        self.beta1=beta1
        self.beta2=beta2
        self.epislon=epislon
        self.m_weight=np.zeros(num_class)
        self.v_weight=np.zeros(num_class)
        self.m_bias=0
        self.v_bias=0
        self.t=0
    
    def set_lr(self, lr):
        self.lr=lr
    
    def reset_weight(self):
        self.weight=np.zeros(self.num_class)
        self.bias=0
    
    def calc(self, x):
        result=np.dot(x, self.weight)
        if not self.no_bias: result+=self.bias
        return 1/(1+np.exp(result))

    def calc_grads(self, x, y):
        val = y-self.calc(x)
        tmp_x = x.transpose()
        det_w = np.sum(val*tmp_x,axis=1)
        if self.no_bias: det_b=0
        else: det_b = sum(val)
        #det_w /= len(x)
        #det_b /= len(x)
        return det_w, det_b
    # 计算梯度

    def cross_entropy(self, x, y):
        score = self.calc(x)
        eps = 1e-10
        return -np.sum(y*np.log(score+eps)+(1-y)*np.log(1-score+eps))
    # 交叉熵

    def predict(self, test_x):
        return self.calc(test_x)

    def score(self, lab1, lab2):
        result=0
        for x,y in zip(lab1,lab2):
            if y<0.5: y=0
            else: y=1
            if x==y: result+=1
        return 1.0*result/len(lab1)
    # 评估

    def fit(self, train_x, train_y, iter_size=5000, batch_size=64, dynamic_lr=False):
        self.reset_weight()
        length = len(train_x)

        if self.optimizer=='BGD': batch_size=length
        else: batch_size = min(batch_size,length)
        p_list = np.linspace(0,length,num=length/batch_size,endpoint=False).astype(int)

        eps = 1e-8; pre_loss=1e18
        for i in range(iter_size):
            for j in p_list:
                R = min(j+batch_size, length)
                det_w, det_b=self.calc_grads(train_x[j:R], train_y[j:R])
                
                if self.optimizer=='Adam':
                    
                    self.t += 1
                    lr = self.lr*(1-self.beta2 ** self.t) ** 0.5 / (1- self.beta1 ** self.t)

                    self.m_weight = self.beta1 * self.m_weight +(1 - self.beta1) * det_w
                    self.v_weight = self.beta2 * self.v_weight +(1 - self.beta2) * (det_w * det_w)
                    self.weight -= lr * self.m_weight /(self.v_weight ** 0.5 + self.epislon)

                    if not self.no_bias:
                        self.m_bias = self.beta1 * self.m_bias +(1 - self.beta1) * det_b
                        self.v_bias = self.beta2 * self.v_bias +(1 - self.beta2) * (det_b * det_b)
                        self.bias -= lr * self.m_bias /(self.v_bias ** 0.5 + self.epislon)
                    # Adam 优化
                else:
                    self.weight -= self.lr * det_w
                    if not self.no_bias: self.bias -= self.lr * det_b
            
            print(self.weight)
            print(self.bias)
            print("epoch %d ---------------"%i)

            '''
            if i%50==0: 
                loss=self.cross_entropy(train_x, train_y)
                if pre_loss-loss<eps ans dynamic_lr==True:
                    if self.lr>eps:
                        print("learning rate change form %f to %f"%(self.lr,self.lr*0.1))
                        self.lr*=0.1
                    else: break
                if i%1000==0: print("epoch %d, loss %f"%(i,loss))
                pre_loss=loss
            '''
            
        print("optimization done, loss %f"%self.cross_entropy(train_x, train_y))
    # 训练
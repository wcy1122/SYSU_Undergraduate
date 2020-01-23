import torch
import torch.nn as nn
import torch.utils.data as Data
from torch.autograd import Variable
import numpy as np
import time
from torch.optim.lr_scheduler import MultiStepLR
import scipy.stats as stats
import pickle

input_size = 50
batch_size = 128
num_layers = 3
hidden_size = 256
epoch_size = 200
# time_step = 15
LR = 1e-3
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

class Timer:
    def __init__(self):
        self.epoch = epoch_size
        self.start_time = time.time()
        self.pre_time = time.time()
        self.epoch = 0
    def update(self):
        self.epoch += 1
        now_time = time.time()
        tot_time = now_time - self.start_time
        expext_time = tot_time * epoch_size / self.epoch
        remain_time = expext_time - tot_time
        epoch_time = now_time - self.pre_time
        self.pre_time = now_time
        print("epoch_time: %02d:%02d:%02d, expect_time: %02d:%02d:%02d, remain_time: %02d:%02d:%02d"%(
                epoch_time/3600, (epoch_time%3600)/60, epoch_time%60,
                expext_time/3600, (expext_time%3600)/60, expext_time%60,
                remain_time/3600, (remain_time%3600)/60, remain_time%60
            )
        )

class RNN(nn.Module):
	def __init__(self,input_size,hidden_size,num_layers):
		super().__init__()
		self.input_size = input_size
		self.hidden_size = hidden_size
		self.num_layers = num_layers
		self.lstm = nn.LSTM(
			input_size=input_size,
			hidden_size=hidden_size,
			num_layers=num_layers,
			batch_first=True,
			bidirectional=True
		)
		self.out = nn.Linear(in_features=256,out_features=10)

	def forward(self,x):
		print(x.shape)
		output,(h_n,c_n) = self.lstm(x)
		# print(output.size())
		output_in_last_timestep = h_n[-1,:,:]
		x = self.out(output_in_last_timestep)
		return x

class my_loss(nn.Module):
	def __init__(self):
		super().__init__()

	def forward(self,pre1, pre2, y):
		score = (1+Cos(pre1,pre2))*2.5
		loss = torch.mean(torch.pow((score - y),2))
		return loss, score

def get_accuracy(pre,std):
	ans ,_ = stats.pearsonr(pre.cpu().detach().numpy(), std.numpy())
	return ans


def trans(data, std):
	data = torch.FloatTensor(data)
	std = torch.FloatTensor(std)
	data_len = len(data)
	std_len = len(std)
	x = np.linspace(0,data_len-2,std_len).astype(int)
	y = np.linspace(1,data_len-1,std_len).astype(int)
	return data[x], data[y], std

def get_data():
	trainx = pickle.load(open("./lib/trainD.pkl",'rb'))
	trainy = pickle.load(open("./lib/trainR.pkl",'rb'))
	testx = pickle.load(open("./lib/testD.pkl",'rb'))
	testy = pickle.load(open("./lib/testR.pkl",'rb'))
	return trainx, trainy, testx, testy

if __name__ == "__main__":
	# Trans_net = nn.Linear(10,1)
	model = RNN(input_size,hidden_size,num_layers).to(device)
	trainx, trainy, testx, testy = get_data()
	train1, train2, std1 = trans(trainx, trainy)
	test1, test2, std2 = trans(testx, testy)
	print(train1.shape)
	print(std1.shape)

	train_set = Data.TensorDataset(train1, train2, std1)
	train_loader = Data.DataLoader(
		dataset = train_set,
		batch_size = batch_size,
		shuffle = False,
		num_workers = 0
	)
	test_set = Data.TensorDataset(test1, test2, std2)
	test_loader = Data.DataLoader(
		dataset = test_set,
		batch_size = batch_size,
		shuffle = False,
		num_workers = 0
	)
	# std = torch.Tensor([[3.8],[4.6]])

	milestone_list = [50,150]
	optimizer = torch.optim.Adam(model.parameters(),lr = LR)
	lr_scheduler = MultiStepLR(optimizer, milestones=milestone_list, gamma=0.1)

	Cos = nn.CosineSimilarity(dim = 1,eps = 1e-6).to(device)

	loss_f = my_loss()
	# loss_f = nn.MSELoss()


	print(model.parameters)
	Time_calculator = Timer()
	train_len = len(std1)
	avg_loss = 0
	for epoch in range(epoch_size):
		model.train()
		train_ans = torch.Tensor().to(device)
		print('current lr {:.5e}'.format(optimizer.param_groups[0]['lr']))
		
		for step, (input1, input2, labels) in enumerate(train_loader):

			inputs = torch.cat((input1, input2), 0)
			inputs = inputs.to(device)
			labels = labels.to(device)
			data_size = labels.size()[0]

			prediction = model(inputs)
			pre1, pre2 = prediction.split(data_size, 0)
			
			loss, score = loss_f(pre1, pre2 ,labels)
			train_ans = torch.cat((train_ans,score),0)
			avg_loss += loss*data_size

			optimizer.zero_grad()
			loss.backward()
			optimizer.step()

		train_accuracy = get_accuracy(train_ans, std1)
		avg_loss /= train_len
		lr_scheduler.step()
		
		with torch.no_grad():
			test_ans = torch.Tensor().to(device)
			for step, (input1, input2, labels) in enumerate(test_loader):
				model.eval()
				inputs = torch.cat((input1, input2), 0)
				inputs = inputs.to(device)
				labels = labels.to(device)
				data_size = labels.size()[0]

				prediction = model(inputs)
				pre1, pre2 = prediction.split(data_size, 0)
				
				score = (1+Cos(pre1, pre2))*2.5
				test_ans = torch.cat((test_ans, score),0)

		test_accuracy = get_accuracy(test_ans, std2)
		print('Epoch: ', epoch, '| train loss: %.4f' % avg_loss, '| train accuracy: %.2f' % train_accuracy, '| test accuracy: %.2f' % test_accuracy)
		
		Time_calculator.update()

		

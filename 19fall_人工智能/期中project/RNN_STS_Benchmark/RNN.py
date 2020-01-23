import torch
from torch import nn


input_size = 50
hidden_size = 256
num_layers = 1

class Rnn(nn.Module):
	"""docstring for Rnn"""
	def __init__(self, input_size,hidden_size,num_layers):
		super().__init__()
		self.rnn = nn.RNN(
			input_size = input_size,
			hidden_size = hidden_size,
			num_layers	= num_layers,
			nonlinearity = 'relu',
			batch_first = True,
			bidirectional = True
			)
		self.out = nn.Linear(256,10)

	def forward(self,x):
		out,h_n = self.rnn(x)
		
		
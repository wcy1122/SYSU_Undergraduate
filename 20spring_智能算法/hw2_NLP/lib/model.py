import torch
import torch.nn as nn
import torch.nn.functional as F
import numpy as np

class LSTM(nn.Module):
    def __init__(self, in_dim, out_dim, \
                input_size=128, hidden_size=128, 
                num_layers=2, device='cpu', 
                drop_rate=0.5, pretrain_embd=None,
                freeze = False
                ):
        super().__init__()
        self.input_size = input_size
        self.output_size = out_dim
        self.hidden_size = hidden_size
        self.num_layers = num_layers
        self.device = device
        self.embedding = nn.Embedding(in_dim, input_size)
        self.lstm = nn.LSTM(
			input_size = input_size,
			hidden_size = hidden_size,
			num_layers = num_layers,
			batch_first = True,
			bidirectional = True,
            dropout = drop_rate
		)
        self.fc = nn.Linear(hidden_size * 2, out_dim)
        if pretrain_embd is not None:
            self.embedding.weight.data.copy_(torch.from_numpy(pretrain_embd))
            if freeze==True:
                self.embedding.weight.requires_grad = False
    
    def forward(self, inputs, length):
        batch_size = len(length)
        max_length = length.max()
        inputs = inputs[:,:max_length]
        inputs = self.embedding(inputs)
        output, (h_n, c_n) = self.lstm(inputs)
        d_out = torch.FloatTensor(batch_size, self.hidden_size*2).to(self.device)
        for i in range(batch_size):
            d_out[i] = output[i, length[i]-1]
        d_out = self.fc(d_out)
        return d_out

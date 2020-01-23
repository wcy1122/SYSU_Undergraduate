import torch
import torch.nn as nn
import torch.nn.functional as F
import numpy as np

import random

class NMT(nn.Module):
    def __init__(self, in_dim, out_dim, input_size=128, hidden_size=128 , 
                num_layers=2, ratio=0.5, ues_attention=True,
                w2v_train = None, w2v_test = None):
        super().__init__()
        self.input_size = input_size
        self.output_size = out_dim
        self.hidden_size = hidden_size
        self.num_layers = num_layers
        self.ratio = ratio
        self.ues_attention = ues_attention
        self.encode_embedding = nn.Embedding(in_dim, input_size)
        self.decode_embedding = nn.Embedding(out_dim, input_size)
        self.encoder_lstm = nn.LSTM(
			input_size = input_size,
			hidden_size = hidden_size,
			num_layers = num_layers,
			batch_first = True,
			bidirectional = True
		)
        self.decoder_lstm = nn.LSTMCell(
			input_size = hidden_size,
			hidden_size = hidden_size,
		)
        self.atten_fc = nn.Linear(hidden_size, hidden_size)
        self.decoder_fc = nn.Linear(2*hidden_size, hidden_size)
        self.out_fc = nn.Linear(hidden_size, out_dim)
        
        if w2v_train is not None:
            print('w2v_train load done.')
            self.encode_embedding.weight.data.copy_(torch.from_numpy(w2v_train))
        if w2v_test is not None:
            print('w2v_test load done.')
            self.decode_embedding.weight.data.copy_(torch.from_numpy(w2v_test))
        
    
    def train_step(self, inputs, labels):
        inputs = self.encode_embedding(inputs)
        # (batch_size, timesteps, hidden_size)
        encoder_outputs, hidden = self.Encoder(inputs)
        # (batch_size, timesteps, hidden_size), (batch_size, hidden_size)
        labels = labels.transpose(0,1)
        # (timesteps, batch_size)
        
        time_steps, batch_size = labels.size()
        result = []
        for step in range(time_steps-1):
            if step==0:
                inputs = torch.zeros(batch_size).long().cuda()
            elif self.training and random.random()<self.ratio:
                inputs = labels[step]
            else:
                _,inputs = torch.max(decoder_out, 1)
            # (batch_size)
            hidden = self.Decoder(inputs, hidden, encoder_outputs)
            # (batch_size, hidden_size)*2
            decoder_out = self.out_fc(hidden[0])
            # (batch_size, out_dim)
            result.append(decoder_out)
            # (x, batch_size, out_dim)
        
        return torch.stack(result).transpose(0,1)

    def test_step(self, inputs, beam_size=5):
        inputs = self.encode_embedding(inputs)
        # (1, timesteps, hidden_size)
        encoder_outputs, hidden = self.Encoder(inputs)
        # (1, timesteps, hidden_size), (1, hidden_size)
        
        time_steps = inputs.size()[1]*2
        cur_beam = [([0], hidden, 1.0)]
        # input, hidden, score
        best_result = None
        max_pro = 0
        for step in range(time_steps):
            candidate_beam = []
            for beam in cur_beam:
                sent, hidden, value = beam
                inputs = torch.LongTensor([sent[-1]]).cuda()
                # (1)
                hidden = self.Decoder(inputs, hidden, encoder_outputs)
                # (1, hidden_size)*2
                decoder_out = F.softmax(self.out_fc(hidden[0]))
                # (1, out_dim)
                val, pos = torch.topk(decoder_out.view(-1).data, k=beam_size)
                for v,p in zip(val,pos):
                    now = sent+[p.item()]
                    pro = value*v
                    if p.item()==1:
                        if pro > max_pro:
                            best_result,max_pro = now,pro
                    else:
                        candidate_beam.append((now, hidden, pro))
            # get candidate beam
            cur_beam = []
            for sent, hidden, pro in candidate_beam:
                if pro >= max_pro:
                    cur_beam.append((sent, hidden, pro))
            # get current beam
            if len(cur_beam) > beam_size:
                probs = [beam[2] for beam in cur_beam]
                pros_idx = np.array(probs).argsort()[-1*beam_size:]
                results = [cur_beam[idx] for idx in pros_idx]
                cur_beam = results
            # sort current beam
            if len(cur_beam)==0: break
        
        if best_result is None:
            best_result = cur_beam[0][0]
        return best_result
        

    def forward(self, inputs, labels=None, beam_size=0):
        if beam_size==0:
            return self.train_step(inputs, labels)
        else:
            return self.test_step(inputs, beam_size)
    
    def Encoder(self, inputs):
        output, (h_n, c_n) = self.encoder_lstm(inputs)
        # (batch_size, timesteps, 2*hidden_size), (batch_size, 2*hidden_size)
        output = output[:, :, :self.hidden_size] + output[:, :, self.hidden_size:]
        # (batch_size, timesteps, hidden_size)
        h_n = (h_n[0] + h_n[1])
        c_n = (c_n[0] + c_n[1])
        # (batch_size, hidden_size) simple add
        return output, (h_n, c_n)

    def Decoder(self, inputs, in_hidden, encoder_outputs):
        inputs = self.decode_embedding(inputs)
        # (batch_size, hidden_size)
        batch_size = inputs.size()[0]

        out_h, out_c = self.decoder_lstm(inputs, in_hidden)
        # (batch_size, hidden_size), (batch_size, hidden_size)
        if self.ues_attention:
            attention = self.attention(out_h, encoder_outputs)
            # (batch_size, 1, timesteps)
            context = torch.bmm(attention, encoder_outputs).squeeze()
            if batch_size==1:
                context = context.unsqueeze(0)
            # (batch_size, hidden_size)
            context = torch.cat((context, out_h), 1)
            # (batch_size,  2*hidden_size)
            out_h = torch.tanh(self.decoder_fc(context))
            # (batch_size,  hidden_size)
        return (out_h, out_c)
    
    def attention(self, ht, hs):
        whs = self.atten_fc(hs).transpose(1,2)
        # (batch_size, hidden_size, timesteps)
        ht = ht.unsqueeze(1)
        # (batch_size, 1, hidden_size)
        score = torch.bmm(ht, whs)
        # (batch_size, 1, timesteps)
        return F.softmax(score, dim=-1)

class MyLoss(nn.Module):
    def __init__(self, weight):
        super().__init__()
        self.Loss_function = nn.CrossEntropyLoss(weight=weight)

    def forward(self, outputs, lables, masks):
        pos = torch.stack([x for x in masks.nonzero() if x[1]>0])
        x,y = pos.transpose(0,1)
        _,m,k = outputs.size()
        outputs = outputs.reshape([-1,k])
        index_pre = x*m+y-1
        predict = torch.index_select(outputs,0,index_pre)
        lables = lables.flatten()
        index_lab = x*(m+1)+y
        gt = torch.index_select(lables,0,index_lab)
        return self.Loss_function(predict, gt)
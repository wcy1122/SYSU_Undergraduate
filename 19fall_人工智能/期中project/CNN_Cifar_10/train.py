import torch
import torch.nn as nn
from torch.optim import Adam, SGD
from torchvision import transforms
from torch.optim.lr_scheduler import MultiStepLR
import numpy as np
from PIL import Image
import pylab
import os
import dataset
from resnet import ResNet18, ResNet34, ResNet50, ResNet101
from vgg import VGG11, VGG13, VGG16
from cnn import CNN
import argparse
import time

parser = argparse.ArgumentParser(description='PyTorch CIFAR-10 Training')
parser.add_argument('--epoch', default=200, type=int, help='epoch_num')
parser.add_argument('--batch', default=128, type=int, help='batch_size')
parser.add_argument('--lr', default=0.1, type=float, help='learning_rate')
parser.add_argument('--lr_decay', default=0.1, type=float, help='learning_rate_decay')
parser.add_argument('--dataset', default='CIFAR10', type=str, help='dataset')
parser.add_argument('--enhance', default=True, type=str, help='data_enhance')
parser.add_argument('--save_dir', default="Output", type=str, help='save_dir')
parser.add_argument('--model', default="ResNet18", type=str, help='model_name')
args = parser.parse_args()

print("train with args:")
print(args)

epoch = args.epoch
batch_size = args.batch
save_dir = args.save_dir

def get_model(model_name):
    if model_name=='CNN': return CNN()
    if model_name=='CNN_GAP': return CNN(GAP=True)
    if model_name=='VGG16': return VGG16(batch_norm=False)
    if model_name=='VGG11_BN': return VGG11(batch_norm=True)
    if model_name=='VGG13_BN': return VGG13(batch_norm=True)
    if model_name=='VGG16_BN': return VGG16(batch_norm=True)
    if model_name=='VGG11_GAP': return VGG11(batch_norm=True, GAP=True)
    if model_name=='VGG13_GAP': return VGG13(batch_norm=True, GAP=True)
    if model_name=='VGG16_GAP': return VGG16(batch_norm=True, GAP=True)
    if model_name=='ResNet18': return ResNet18()
    if model_name=='ResNet34': return ResNet34()
    if model_name=='ResNet50': return ResNet50()
    if model_name=='ResNet101': return ResNet101()
    raise NotImplementedError('Model has not been implement.')

def get_dir(output_dir, file_name):
    return os.path.join(output_dir, file_name)

def save_data(num, file_name):
    out_dir = get_dir(save_dir, 'log')
    out_dir = get_dir(out_dir, file_name)
    out_file = open(out_dir, "a+")
    print("%f"%num, file = out_file)
    out_file.close()

def save_result(epoch=0, iter=-1, acc=0, loss=0, stype='train'):
    if stype=='train':
        if iter<0:
            save_data(acc, "epoch_accuracy.out")
            print("[epoch %d]: train_accuracy: %f loss: %f"%(epoch, acc, loss))
        else:
            save_data(acc, "train_accuracy.out")
            save_data(loss, "train_loss.out")
            if iter>0 and iter%100==0:
                print("[epoch %d iter %d]: loss: %f, accuracy: %f"%(epoch, iter , loss, acc))
    else:
        save_data(acc, "test_accuracy.out")
        print("[epoch %d]: test_accuracy: %f"%(epoch, acc))

def init_output_log():
    if not os.path.isdir(save_dir):
        os.mkdir(save_dir)
    log_dir = os.path.join(save_dir, 'log')
    if not os.path.isdir(log_dir):
        os.mkdir(log_dir)
    file_list = [
        'epoch_accuracy.out', 
        'train_accuracy.out', 
        'train_loss.out', 
        'test_accuracy.out'
    ]
    for file_name in file_list:
        file_name = get_dir(log_dir, file_name)
        out_file = open(file_name, "w")
        out_file.close()

    checkpoint_dir = os.path.join(save_dir, 'checkpoint')
    if not os.path.isdir(checkpoint_dir):
        os.mkdir(checkpoint_dir)

def save_check_point(net, optimizer, file_name):
    state = {
        'net_state_dict': net.state_dict(),
        'optimizer_state_dict': optimizer.state_dict()
    }
    checkpoint_dir = os.path.join(save_dir, 'checkpoint')
    save_path = os.path.join(checkpoint_dir, file_name)
    torch.save(state, save_path)

class Timer:
    def __init__(self):
        self.epoch = args.epoch
        self.start_time = time.time()
        self.pre_time = time.time()
        self.epoch = 0
    def update(self):
        self.epoch += 1
        now_time = time.time()
        tot_time = now_time - self.start_time
        expext_time = tot_time * args.epoch / self.epoch
        remain_time = expext_time - tot_time
        epoch_time = now_time - self.pre_time
        self.pre_time = now_time
        print("epoch_time: %02d:%02d:%02d, expect_time: %02d:%02d:%02d, remain_time: %02d:%02d:%02d"%(
                epoch_time/3600, (epoch_time%3600)/60, epoch_time%60,
                expext_time/3600, (expext_time%3600)/60, expext_time%60,
                remain_time/3600, (remain_time%3600)/60, remain_time%60
            )
        )

train_loader = dataset.get_train(batch_size, args.enhance)
val_loader = dataset.get_val(batch_size)
print('load training set done')

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
net = get_model(args.model).to(device)
Loss_function = nn.CrossEntropyLoss()

milestone_list = [100, 150]
optimizer = SGD(net.parameters(), lr=args.lr, momentum=0.9, weight_decay=1e-4)
lr_scheduler = MultiStepLR(optimizer, milestones=milestone_list, gamma=args.lr_decay)
net = nn.DataParallel(net)

init_output_log()

print("training start.")
best_accuracy = 0.0
Time_calculator = Timer()
for i in range(epoch):
    net.train()
    train_accuracy = 0; sum_loss = 0; train_len = 0; train_batch = 0

    for j, (inputs, labels) in enumerate(train_loader):
        inputs = inputs.to(device, dtype=torch.float32)
        labels = labels.to(device, dtype=torch.int64)
        
        optimizer.zero_grad()
        outputs = net(inputs)
        loss = Loss_function(outputs, labels)
        loss.backward()
        optimizer.step()

        _,predicted = torch.max(outputs.data,1)
        count = predicted.eq(labels.data).cpu().sum()
        
        accuracy = 100.0*count.double()/len(inputs)
        save_result(epoch=i, iter=j, acc=accuracy, loss=loss, stype='train')

        train_len += len(inputs)
        sum_loss += loss
        train_accuracy += count.double()
        train_batch += 1
    
    print('current lr {:.5e}'.format(optimizer.param_groups[0]['lr']))
    lr_scheduler.step()
        
    train_accuracy = 100.0*train_accuracy/train_len
    avg_loss = sum_loss/train_batch
    save_result(epoch=i, acc=train_accuracy, loss=avg_loss, stype='train')
        
    test_accuracy = 0; test_len = 0
    with torch.no_grad():
        for j, (inputs, labels) in enumerate(val_loader):
            net.eval()
            inputs = inputs.to(device, dtype=torch.float32)
            labels = labels.to(device, dtype=torch.int64)

            outputs=net(inputs)
            
            _,predicted = torch.max(outputs.data,1)
            test_accuracy += predicted.eq(labels.data).cpu().sum().double()
            test_len += len(inputs)
        
        test_accuracy = 100.0*test_accuracy/test_len
        save_result(epoch=i, acc=test_accuracy, stype='test')
    
    if i in milestone_list:
        save_check_point(net, optimizer, '%d.pth'%i)
    if test_accuracy>best_accuracy:
        save_check_point(net, optimizer, 'CIFAR_10.pth')
        best_accuracy = test_accuracy
        print("========== < save checkpoint with acc %f > =========="%best_accuracy)
    Time_calculator.update()
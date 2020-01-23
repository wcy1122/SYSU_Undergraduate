import torch
import torch.nn as nn
from torchvision import transforms
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

parser = argparse.ArgumentParser(description='PyTorch CIFAR-10 Testing')
parser.add_argument('--batch', default=128, type=int, help='batch_size')
parser.add_argument('--dataset', default='CIFAR10', type=str, help='dataset')
parser.add_argument('--load_dir', default="Output", type=str, help='load_dir')
parser.add_argument('--model', default="ResNet18", type=str, help='model_name')
args = parser.parse_args()
load_dir = args.load_dir
model = args.model
batch_size = args.batch
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")


lable={
    0:'cat',1:'frog',2:'truck',3:'deer',4:'automobile',
    5:'bird',6:'horse',7:'ship',8:'airplane',9:'dog'
}

print("test with args:")
print(args)


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

'''
def get_train_log():
    log_dir = os.path.join(load_dir, 'log')
    file_list = [
        'epoch_accuracy.out', 
        'train_accuracy.out', 
        'train_loss.out', 
        'test_accuracy.out'
    ]
    for file_name in file_list:
        file_name = get_dir(log_dir, file_name)
        out_file = open(file_name, "r")
'''

def load_trained_weight(model):
    weight_dir = os.path.join(load_dir, 'checkpoint')
    weight_dir = os.path.join(weight_dir, 'CIFAR_10.pth')
    if device=='cuda': weight_dict = torch.load(weight_dir)
    else: weight_dict = torch.load(weight_dir, map_location='cpu')
    model.load_state_dict(weight_dict['net_state_dict'])
    return model

train_loader = dataset.get_train(batch_size, False)
test_loader = dataset.get_val(batch_size)
print("data load done.")

model = get_model(model).to(device)
model = nn.DataParallel(model)
model = load_trained_weight(model)
print("model load done")

model.eval()

confuse_matrix = np.zeros([10,10])
class_accuracy = np.zeros(10)
test_accuracy = 0; test_len = 0
for i, (inputs, labels) in enumerate(train_loader):
    inputs = inputs.to(device, dtype=torch.float32)
    labels = labels.to(device, dtype=torch.int64)
    outputs = model(inputs)
    _, predicted = torch.max(outputs.data,1)
    test_accuracy += predicted.eq(labels.data).cpu().sum().double()
    test_len += len(inputs)
    for x,y in zip(predicted, labels):
        if x==y: class_accuracy[x]+=1
        confuse_matrix[x][y]+=1
print(class_accuracy/5000.0)
print(confuse_matrix/5000.0)
test_accuracy = 100.0*test_accuracy/test_len
print(test_accuracy)
print("=====train dataset done=====")

confuse_matrix = np.zeros([10,10])
class_accuracy = np.zeros(10)
test_accuracy = 0; test_len = 0
for i, (inputs, labels) in enumerate(test_loader):
    inputs = inputs.to(device, dtype=torch.float32)
    labels = labels.to(device, dtype=torch.int64)
    outputs = model(inputs)
    _, predicted = torch.max(outputs.data,1)
    test_accuracy += predicted.eq(labels.data).cpu().sum().double()
    test_len += len(inputs)
    for x,y in zip(predicted, labels):
        if x==y: class_accuracy[x]+=1
        confuse_matrix[x][y]+=1
print(class_accuracy/1000.0)
print(confuse_matrix/1000.0)
test_accuracy = 100.0*test_accuracy/test_len
print(test_accuracy)
print("=====test dataset done=====")
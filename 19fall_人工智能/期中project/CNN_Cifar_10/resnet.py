import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import torchvision
import torchvision.transforms as transforms


class BasicBlock(nn.Module):
    expansion = 1
    def __init__(self, dim_in, dim_out, Residual=True, stride=1):
        super(BasicBlock, self).__init__()
        self.left = nn.Sequential(
            nn.Conv2d(dim_in, dim_out, kernel_size=3, stride=stride, padding=1, bias=False),
            nn.BatchNorm2d(dim_out),
            nn.ReLU(inplace=True),
            nn.Conv2d(dim_out, dim_out, kernel_size=3, stride=1, padding=1, bias=False),
            nn.BatchNorm2d(dim_out)
        )
        self.Residual = Residual
        self.shortcut = nn.Sequential()
        if stride!=1 or dim_in!=dim_out:
            self.shortcut = nn.Sequential(
                nn.Conv2d(dim_in, dim_out, kernel_size=1, stride=stride, bias=False),
                nn.BatchNorm2d(dim_out)
            )
    
    def forward(self, x):
        out = self.left(x)
        if self.Residual: out += self.shortcut(x)
        out = F.relu(out)
        return out

class BottleNeck(nn.Module):
    expansion = 4
    def __init__(self, dim_in, dim_out, Residual=True, stride=1):
        super(BottleNeck, self).__init__()
        self.left = nn.Sequential(
            nn.Conv2d(dim_in, dim_out, kernel_size=1, bias=False),
            nn.BatchNorm2d(dim_out),
            nn.ReLU(inplace=True),
            nn.Conv2d(dim_out, dim_out, stride=stride, kernel_size=3, padding=1, bias=False),
            nn.BatchNorm2d(dim_out),
            nn.ReLU(inplace=True),
            nn.Conv2d(dim_out, dim_out * BottleNeck.expansion, kernel_size=1, bias=False),
            nn.BatchNorm2d(dim_out * BottleNeck.expansion),
        )
        self.Residual = Residual
        self.shortcut = nn.Sequential()
        if stride != 1 or dim_in != dim_out * BottleNeck.expansion:
            self.shortcut = nn.Sequential(
                nn.Conv2d(dim_in, dim_out * BottleNeck.expansion, stride=stride, kernel_size=1, bias=False),
                nn.BatchNorm2d(dim_out * BottleNeck.expansion)
            )
        
    def forward(self, x):
        out = self.left(x)
        if self.Residual: out += self.shortcut(x)
        out = F.relu(out)
        return out

class ResNet(nn.Module):
    def __init__(self, Block, num_blocks, Residual=True, num_classes=10):
        super(ResNet, self).__init__()
        chennels=[64, 128, 256, 512]
        
        self.conv1 = nn.Sequential(
            nn.Conv2d(3, chennels[0], kernel_size=3, stride=1, padding=1, bias=False),
            nn.BatchNorm2d(chennels[0]),
            nn.ReLU(),
        )
        self.dim_in = chennels[0]
        self.layer1 = self.make_layer(Block, chennels[0], num_blocks[0], Residual, stride=1)
        self.layer2 = self.make_layer(Block, chennels[1], num_blocks[1], Residual, stride=2)
        self.layer3 = self.make_layer(Block, chennels[2], num_blocks[2], Residual, stride=2)
        self.layer4 = self.make_layer(Block, chennels[3], num_blocks[3], Residual, stride=2)
        self.avg_pool = nn.AdaptiveAvgPool2d((1, 1))
        self.fc = nn.Linear(chennels[-1] * Block.expansion, num_classes)
    
    def make_layer(self, block, channels, num_blocks, Residual, stride):
        strides=[stride]+[1]*(num_blocks-1)
        layers=[]
        for stride in strides:
            layers.append(block(self.dim_in, channels, Residual, stride))
            self.dim_in = channels * block.expansion
        return nn.Sequential(*layers)
    
    def forward(self, x):
        x = self.conv1(x)
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)
        x = self.avg_pool(x)
        x = x.view(x.size(0), -1)
        x = self.fc(x)
        return x

def ResNet18():
    return ResNet(Block=BasicBlock, num_blocks=[2,2,2,2])

def ResNet34():
    return ResNet(Block=BasicBlock, num_blocks=[3,4,6,3])

def ResNet50():
    return ResNet(Block=BottleNeck, num_blocks=[3,4,6,3])

def ResNet101():
    return ResNet(Block=BottleNeck, num_blocks=[3,4,23,3])

import torch
import torch.nn as nn
import torch.nn.functional as F

class VGGNet(nn.Module):
    def __init__(self, num_classes=10, blocks=[], batch_norm=True, GAP=False):
        super(VGGNet, self).__init__()
        self.conv = self.make_layer(blocks, 3, batch_norm)
        self.GAP=GAP
        if GAP==True:
            self.avg_pool = nn.AdaptiveAvgPool2d((1, 1))
            self.classifier = nn.Sequential(
                nn.Linear(512, num_classes)
            )
        else:
            self.classifier = nn.Sequential(
                nn.Linear(512, 4096),
                nn.ReLU(inplace=True),
                nn.Linear(4096, 4096),
                nn.ReLU(inplace=True),
                nn.Linear(4096, num_classes)
            )

    def make_layer(self, cfg, in_channels, batch_norm=True):
        layers = []
        for x in cfg:
            if x == 'M':
                layers += [nn.MaxPool2d(kernel_size=2, stride=2)]
            else:
                conv2d = nn.Conv2d(in_channels, x, kernel_size=3, stride=1, padding=1, bias=True)
                if batch_norm: layers += [conv2d, nn.BatchNorm2d(x), nn.ReLU(inplace=True)]
                else: layers += [conv2d, nn.ReLU(inplace=True)] 
                in_channels = x
        return nn.Sequential(*layers)

    def forward(self, x):
        x = self.conv(x)
        if self.GAP==True:
            x = self.avg_pool(x)
        x = x.view(x.size(0), -1)
        x = self.classifier(x)
        return x


def VGG11(batch_norm=True, GAP=False):
    return VGGNet(
        blocks=[64, 'M', 128, 'M', 256, 256, 'M', 512, 512, 'M', 512, 512, 'M'],
        batch_norm=batch_norm, GAP=GAP
    )

def VGG13(batch_norm=True, GAP=False):
    return VGGNet(
        blocks=[64, 64, 'M', 128, 128, 'M', 256, 256, 'M', 512, 512, 'M', 512, 512, 'M'],
        batch_norm=batch_norm, GAP=GAP
    )

def VGG16(batch_norm=True, GAP=False):
    return VGGNet(
        blocks=[64, 64, 'M', 128, 128, 'M', 256, 256, 256, 'M', 512, 512, 512, 'M', 512, 512, 512, 'M'],
        batch_norm=batch_norm, GAP=GAP    
    )
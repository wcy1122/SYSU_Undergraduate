import torch.utils.data as data
import torchvision.datasets as datasets
from torchvision import transforms

transforms1 = transforms.Compose([
    transforms.RandomHorizontalFlip(),
    transforms.RandomCrop(32, 4),
    transforms.ToTensor(),
    transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
])

transforms2 = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
])

def get_train(batch_size, enhance):
    transform_train = transforms2
    if enhance: transform_train = transforms1
    train_loader = data.DataLoader(
        datasets.CIFAR10(
            root = './dataset',
            train = True,
            transform = transform_train,
            download = True
        ),
        batch_size=batch_size,
        shuffle=True,
        num_workers=0,
        pin_memory=True
    )
    return train_loader

def get_val(batch_size):
    transform_test = transforms2
    val_loader = data.DataLoader(
        datasets.CIFAR10(
            root='./dataset', 
            train=False, 
            transform = transform_test,
            download = True
        ),
        batch_size=batch_size,
        shuffle=False,
        num_workers=0,
        pin_memory=True
    )
    return val_loader
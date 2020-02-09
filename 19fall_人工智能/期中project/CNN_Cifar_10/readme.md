# CNN_CIFAR-10

使用CIFAR-10数据集，包含50000张训练图像和10000张测试图像。<br>
使用 简单CNN, VGGNet, ResNet 三种模型进行训练，最高可以获得接近95%的准确率。<br>

| 模型     | 分类器 | 准确率 |
| :---:    | :---:  | :---:  |
| CNN      | FC     | 81.69  | 
| CNN      | GAP    | 80.85  | 
| VGG11    | FC     | 91.82  |
| VGG11    | GAP    | 92.09  |
| VGG16    | FC     | 93.81  |
| VGG16    | GAP    | 93.97  |
| VGG19    | FC     | 93.44  |
| VGG19    | GAP    | 93.61  |
| ResNet18 | GAP    | 94.62  |
| ResNet34 | GAP    | 94.90  |
| ResNet50 | GAP    | 94.59  |

# 数据集
建议提前下载数据集，并解压至 ./dataset<br>

# 超参
- batch: batch size
- epoch: epoch size
- lr: 学习率
- save_dir: 保存地址
- model: 模型名称

# 运行
在0号显卡训练ResNet18
```
CUDA_VISIBLE_DEVICE=0 python train.py --model RseNet18 --save_dir ResNet18
```
支持multi-gpu训练，指定多张显卡即可，比如
```
CUDA_VISIBLE_DEVICE=0,1
```
测试
```
CUDA_VISIBLE_DEVICE=0 python test.py --model RseNet18 --load_dir ResNet18
```

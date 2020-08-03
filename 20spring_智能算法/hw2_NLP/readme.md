# Sentiment Analysis on Movie Reviews

**Author:** wcy1122 (Chengyao Wang)
**Language:** python
**Requirement:** pytorch, numpy, matplotlib, cuda (not necessary)

### Args

#### Train

| arg | mean | default |
| --- | --- | --- |
| epoch | epoch size | 100 |
| batch | batch size | 128 |
| embed | embedding size | 50 |
| init_LR | initial learning rate | 50 |
| out_dir | save dir | 50 |
| drop_rate | dropout rate | 0 |
| num_layers | num layers | 2 |
| load_embd | pretrain model filename | None |
| freeze | if freeze pretrain layer | False |
| weighted | if use weighted sampler | False |
| freeze | split rate for train/valid | 0.8 |

#### Test

| arg | mean | default |
| --- | --- | --- |
| batch | batch size | 128 |
| embed | embedding size | 50 |
| in_dir | load dir | LSTM_50 |
| step | model step | 0 |
| dataset | dataset type | test |
| sent_only | is test sentence only | False |
- if step==0, you will load ```best.pth```
- dataset can be set with train/val/test, to test on different dataset
- if sent_only==True, you will only test on data with the whole sentence

### Usage

#### Dataset

You should put ```train.tsv``` and ```test.tsv``` on ```Base_dir/data/```.
If you want to use pretrain model, you should put them on ```Base_dir/data/pretrain/```

#### Train
Here is the command to train a model with 50d embadding vectors pretrained by Glove and dropout on GPU0. 

Since it doesn't need too much VRAM (only about 1G), this code doesn't support multi-GPU train/test.

The Output logs ans checkpoint will be saved in ```Base_dir/lstm_glove_50/```.

```
CUDA_VISIBLE_DEVICES=0 python ./src/train.py \
	--out_dir lstm_glove_50 --drop_rate 0.5 \
	--load_embd glove.6B.50d.txt --embed 50
```

#### Train
Here is the command to test the model above on GPU0.
```
CUDA_VISIBLE_DEVICES=0 python ./src/test.py \
	--in_dir lstm_glove_50 --embed 50
```

### Sources code
all the soucre code are in ```./src```
```train.py```: to train a model
```test.py```: to test a model
```model.py```: implement neural network modle by pytorch
```dataloader.py```: load dataset
```utils.py```: some useful code, like save model, save log, split dataset
```times.py```: show running time
```log.py```: draw curve

### Hint

- There is no ```./bin``` since we use python.
- There is no ```./tc``` since the dataset is too large.
- We provide our final result, it's in ```./result/Final_result.csv```
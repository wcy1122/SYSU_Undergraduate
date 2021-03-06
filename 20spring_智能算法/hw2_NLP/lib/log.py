import os
import matplotlib.pyplot as plt

model_list=[
    'baseline',
    'baseline+drop',
    'baseline+glove',
    'baseline+glove+drop'
]

file_list = [
    'train_loss.txt', 
	'valid_loss.txt'
]

def plot_result(X,Y,label):
    plt.plot(X,Y,label=label)

def get_dir(output_dir, file_name):
    return os.path.join(output_dir, file_name)

plt.title('Loss Value with different drop_rate')
plt.xlabel('epoch')
plt.ylabel('Loss')

for model in model_list:
    log_dir = os.path.join('..\result', model, 'log')
    
    for fname in file_list:
        file_name = get_dir(log_dir, fname)
        log_file = open(file_name, "r")
        title = file_name.split('.')[0]
        data=[float(x) for x in log_file.readlines()]
        plot_result(range(len(data)),data,title)

plt.legend()
plt.show()
import numpy as np
import random
import matplotlib.pyplot as plt
from genetic import Genetic
from local import LocalSearch
import utils
import argparse

parser = argparse.ArgumentParser(description='TSP')
parser.add_argument('--in_dir', default='berlin52.tsp', type=str, help='data path')
parser.add_argument('--method', default='annealing', type=str, help='method')
parser.add_argument('--seed', default=666072, type=int, help='seed')
args = parser.parse_args()
print("Call with args:")
print(args)

def load_data(in_dir):
    in_file = open(in_dir, "r")
    lines = in_file.readlines()
    Position = []
    for line in lines:
        line = line.rstrip("\n").split(' ')
        _, x, y = [x for x in line if x != '']
        Position.append([float(x), float(y)])
    return np.array(Position)

def main():
    in_dir = args.in_dir
    seed = args.seed
    method = args.method
    
    random.seed(seed)
    np.random.seed(seed)
    Position = load_data(in_dir)
    
    method = 'annealing'
    if method=='local' or method=='annealing':
        ans, Route = LocalSearch(Position, method=method)
    elif method=='genetic':
        ans, Route = Genetic(Position)
    print(ans)

if __name__ == '__main__':
    main()


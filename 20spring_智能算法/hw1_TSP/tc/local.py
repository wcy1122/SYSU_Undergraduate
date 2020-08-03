import numpy as np
import random
import matplotlib.pyplot as plt
from utils import draw, CalcDis

def Work(Route):
    length = len(Route)
    x = random.randint(0, length-1)
    y = random.randint(0, length-1)
    z = random.randint(0, length-1)
    if x==y: return Route
    if x > y: x, y = y, x
    prob = random.random()
    if prob<0.5:
        if prob<0.25:
            Route = np.hstack((np.hstack((Route[0:x+1], Route[y:])), Route[x+1:y]))
        else:
            Route = np.hstack((np.hstack((Route[0:x+1], Route[x+1:y+1][::-1])), Route[y+1:]))
    else:
        if x > z: x, z = z, x
        if y > z: y, z = z, y
        if x==y or y==z: return Route
        if prob<0.75:
            Route = np.hstack((np.hstack((np.hstack((Route[0:x+1], Route[y:z+1])), Route[x+1:y])), Route[z+1:]))
        else:
            Route = np.hstack((np.hstack((np.hstack((Route[0:x+1], Route[y:z+1])), Route[x+1:y][::-1])), Route[z+1:]))
    return Route

def Local(Position, ans, Route):
    for i in range(100):
        Route_new = Work(Route.copy())
        ans_new = CalcDis(Position, Route_new)
        if ans_new < ans:
            ans, Route = ans_new, Route_new
    return ans, Route

def Anneal(Position, ans_k, Route_k, ans, Route, Temp):
    for i in range(100):
        Route_new = Work(Route_k.copy())
        ans_new = CalcDis(Position, Route_new)
        if ans_new < ans_k:
            ans_k, Route_k = ans_new, Route_new
            if ans_k < ans:
                ans, Route = ans_k, Route_k
        else:
            P = np.exp(-(ans_new - ans_k)/Temp)
            if random.random()<P:
                ans_k, Route_k = ans_new, Route_new
    return ans_k, Route_k, ans, Route

def LocalSearch(Position, method='local'):
    num_node = len(Position)
    Route = np.arange(num_node)
    np.random.shuffle(Route)
    ans = CalcDis(Position, Route)
    # init random answer

    plt.ion()
    _, axs = plt.subplots(1)
    draw(Position, Route, axs, 0, ans)

    Count = 1000
    if method == 'annealing': 
        Temp0 = 50; ans_k, Route_k = ans, Route
    ans_list = []
    for i in range(Count):
        if method == 'local': 
            ans, Route = Local(Position, ans, Route)
        if method == 'annealing': 
            Temp = Temp0 if i==0 else Temp0 / np.log(1 + i)
            ans_k, Route_k, ans, Route = \
                Anneal(Position, ans_k, Route_k, ans, Route, Temp)
        ans_list.append(ans)
        draw(Position, Route, axs, i, ans)
        plt.pause(0.0001)
    
    plt.ioff()
    plt.show()

    plt.plot(np.arange(Count), ans_list)
    plt.show()

    return ans, Route
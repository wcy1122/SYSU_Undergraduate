import numpy as np
import random
import matplotlib.pyplot as plt

def draw(Position, Route, axs, p, ans):
    plt.cla()
    plt.suptitle("step %d, ans %.4f"%(p, ans), fontsize=20)
    Route_t = np.hstack((Route, Route[0]))
    axs.plot(Position[Route_t,0], Position[Route_t,1],
            marker='o', markerfacecolor='b', color='c', linestyle='-')
    plt.show()

def CalcDis(Position, Route):
    num_nodes = len(Position)
    Route_t = np.hstack((Route, Route[0]))
    Begin = Position[Route_t[0:num_nodes]]
    End = Position[Route_t[1:num_nodes+1]]
    ans = np.sqrt(((Begin - End)**2).sum(axis=1)).sum()
    return ans
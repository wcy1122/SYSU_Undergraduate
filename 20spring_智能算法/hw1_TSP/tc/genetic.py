import numpy as np
import random
import matplotlib.pyplot as plt
from utils import draw, CalcDis
from local import Work

def Eval(Position, Routes):
    num_routes = len(Routes)
    AnsAll = []
    for i in range(num_routes):
        AnsAll.append(CalcDis(Position, Routes[i]))
    return np.stack(AnsAll)

def Select(Routes, AnsAll, step):
    if step<200: rate = 8
    else: rate = 4
    FitScore = 1 / (AnsAll**rate)
    SelectProb = FitScore / FitScore.sum()
    SumProb = np.add.accumulate(SelectProb)
    Count = len(Routes)
    idxs = []    
    for i in range(Count):
        prob = random.random()
        idxs.append(len(np.where(SumProb < prob)[0]))
    return Routes[idxs]

def Find(Routes, x, y, pos):
    while True:
        p = np.where(Routes[x]==pos)[0]
        if len(p)==0: return pos
        pos = Routes[y, p]

def Mating(Routes):
    num_routes, num_cities = Routes.shape
    prob = 0.5
    mate_idxs = []
    for i in range(num_routes):
        if random.random() < prob:
            mate_idxs.append(i)
    num_mating = len(mate_idxs) // 2
    for i in range(num_mating):
        x, y = mate_idxs[i*2], mate_idxs[i*2+1]
        L = random.randint(0, num_cities)
        R = random.randint(0, min(num_cities-L, 15)) + L
        Routes[x, L:R+1], Routes[y, L:R+1] = \
            Routes[y, L:R+1].copy(), Routes[x, L:R+1].copy()
        # select mating routes
        to = np.arange(num_cities)
        count = np.zeros(num_cities)
        count[Routes[x, L:R+1]] += 1
        count[Routes[y, L:R+1]] += 1
        posto = np.where(count==1)[0]
        for pos in posto:
            p = np.where(Routes[x, L:R+1]==pos)[0]
            if len(p)>0: to[pos] = Find(Routes[:,L:R+1], x, y, Routes[y, L+p])
            else: 
                p = np.where(Routes[y, L:R+1]==pos)[0]
                to[pos] = Find(Routes[:,L:R+1], y, x, Routes[x, L+p])
        # generate to
        except_idxs = np.hstack((np.arange(0, L), np.arange(R+1, num_cities)))
        for j in except_idxs:
            Routes[x, j] = to[Routes[x, j]]
            Routes[y, j] = to[Routes[y, j]]
        # make routes valid by to
        
    return Routes

def Variation(Positions, Routes, step):
    num_routes = len(Routes)
    if step<200: prob = 0.5
    else: prob = 0.1
    for i in range(num_routes):
        if random.random()<prob:
            Route_now = Routes[i]
            Ans_now = CalcDis(Positions, Routes[i])
            for j in range(20):
                Route_tmp = Work(Routes[i].copy())
                Ans_tmp = CalcDis(Positions, Route_tmp)
                if Ans_tmp < Ans_now:
                    Route_now = Route_tmp
                    Ans_now = Ans_tmp
            Routes[i] = Route_now
    return Routes

def Genetic(Position):
    GroupSize = 500
    num_node = len(Position)
    Routes = []
    Route = np.arange(num_node)
    for i in range(GroupSize):
        np.random.shuffle(Route)
        Routes.append(Route)
    Routes = np.stack(Routes)
    # init random answer

    plt.ion()
    _, axs = plt.subplots(1)

    AnsAll = Eval(Position, Routes)
    ans = AnsAll.min()
    Route = Routes[AnsAll.argmin()]
    draw(Position, Route, axs, 0, ans)
    ans_list = []
    Count = 1000
    for i in range(Count):
        Routes = Select(Routes, AnsAll, i)
        # select
        Routes = Mating(Routes)
        # mating
        Routes = Variation(Position, Routes, i)
        # variation
        AnsAll = Eval(Position, Routes)
        if AnsAll.min()<ans:
            ans = AnsAll.min()
            Route = Routes[AnsAll.argmin()]
        # evaluate
        draw(Position, Route, axs, i, ans)
        plt.pause(0.0001)
        ans_list.append(ans)
    
    plt.ioff()
    plt.show()

    plt.plot(np.arange(Count), ans_list)
    plt.show()

    return ans, Route
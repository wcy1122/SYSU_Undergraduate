Traveling Salesman Problem
---
**Author:** wcy1122 (Chengyao Wang)
**Language:** python
**Requirement:** numpy, matplotlib
**Args**

| arg | default |
| --- | --- |
| in_dir | berlin52.tsp |
| method | annealing |
| seed | 666072 |

**Usage**

```
python tsp.py --method=annealing
```
there are two input file in ```./tc```: ```sample.txt```, ```berlin52.tsp```

**Sources code**
all the soucre code are in ```./src```
tsp.py: main function
genetic.py: implement of genetic algorithm
local.py: implement of local search
utils: draw graph and calculate distance

**Hint:** Note that there is no ```./bin``` since we use python
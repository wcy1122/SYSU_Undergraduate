complier homework3 (RE-NFA-DFA-minDFA)
---
17341146 王程钥

**RE**

>(a|b)\*aba(a|b)\*

**NFA**

```mermaid
graph LR
    start[start] --> 0[0]
    0[0] -- ε --> 1[1]
    1[1] -- ε --> 2[2]
    1[1] -- ε --> 4[4]
    2[2] -- a --> 3[3]
    4[4] -- b --> 5[5]
    3[3] -- ε --> 6[6]
    5[5] -- ε --> 6[6]
    6[6] -- ε --> 7[7]
    6[6] -- ε --> 1[1]
    1[1] -- ε --> 7[7]
    7[7] -- a --> 8[8]
    8[8] -- b --> 9[9]
    9[9] -- a --> 10[10]
    
    10[10] -- ε --> 11[11]
    11[11] -- ε --> 12[12]
    12[12] -- a --> 13[13]
    11[11] -- ε --> 14[14]
    14[14] -- b --> 15[15]
    13[13] -- ε --> 16[16]
    15[15] -- ε --> 16[16]
    16[16] -- ε --> 11[11]
    16[16] -- ε --> 17[17]
    11[11] -- ε --> 17[*17]
```

**DFA**

| NFA states | DFA state | a | b |
| - | - | - | - |
| {0,1,2,4,7}| A | B | C |
| {1,2,3,4,6,7,8} | B | B | D |
| {1,2,4,5,6,7} | C | B | C |
| {1,2,4,5,6,7,9} | D | E | C |
| {1,2,3,4,6,7,8,10,11,12,14,17} | E | F | G |
| {1,2,3,4,6,7,8,11,12,13,14,16,17} | F | F | G |
| {1,2,4,5,6,7,9,11,12,14,15,16,17} | G | H | G |
| {1,2,3,4,6,7,8,10,11,12,13,14,16,17} | H | H | G |

```mermaid
graph LR
    start[start] --> A[A]
    A[A] -- a --> B[B]
    A[A] -- b --> C[C]
    B[B] -- a --> B[B]
    B[B] -- b --> D[D]
    C[C] -- a --> B[B]
    C[C] -- b --> C[C]
    D[D] -- a --> E[*E]
    D[D] -- b --> C[C]
    E[*E] -- a --> F[*F]
    E[*E] -- b --> G[*G]
    F[*F] -- a --> F[*F]
    F[*F] -- b --> G[*G]
    G[*G] -- a --> H[*H]
    G[*G] -- b --> G[*G]
    H[*H] -- a --> H[*H]
    H[*H] -- b --> G[*G]
```
**Hint:** *E means E is a terminal node, so are other nodes.

**min-DFA**

Initially, since EFGH are terminal nodes, we set ABCD in group 1, and EFGH in group 2.
For all node pairs in group A, only for node A and C, terminal nodes are in the same group, so we can merge A and C together.
For all node pairs in group B, their terminal points are in the same group, so we can merge all of them together.
Here is the min-DFA.

```mermaid
graph LR
    start[start] --> A[A]
    A[A] -- a --> B[B]
    A[A] -- b --> A[A]
    B[B] -- a --> B[B]
    B[B] -- b --> D[D]
    D[D] -- a --> E[*E]
    D[D] -- b --> A[A]
```

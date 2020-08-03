Complier Homework 4
---
17341146 王程钥

**Problem**
Write down Regular Expression, Regular Grammar and DFA for L.
L is a string consist of even number of letter a and letter b.

**Regular Expression**

> ( (aa|bb) | (ab|ba)(aa|bb)\*(ab|ba) )*

**Regular Grammar**

> S -->  CS
> S --> ε
> A --> aa | bb
> B --> ab | ba
> C --> A | D
> D --> BEBD
> D --> ε
> E --> A
> E --> ε
>

**DFA**

```mermaid
graph LR
  0[0] -- a --> 1[1]
  0[0] -- b --> 2[2]
  1[1] -- a --> 0[0]
  1[1] -- b --> 3[3]
  2[2] -- a --> 3[3]
  2[2] -- b --> 0[0]
  3[3] -- a --> 2[2]
  3[3] -- b --> 1[1]
```
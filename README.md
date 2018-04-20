# 1000000th Customer Prize: another programming challenge

## Introduction
This repo is my solution for [challenge](http://geeks.redmart.com/2015/10/26/1000000th-customer-prize-another-programming-challenge/).
Basically, this is a 0-1 [knapsack problem](https://en.wikipedia.org/wiki/Knapsack_problem) that utilize dynamic programming
paradigm. However, the problem addressed maximization of total price (first priority) and minimization of total weight (second priority).

## List of Problem
- Pick at most 1 from each kind of product
- Total volume <= tote total volume (45 x 30 x 35 = 47250)
- Products must fit individually (a product with dimmension 35 x 35 x 45 cannot fit) 
- Maximize total prices
- Minimize total weights

## Implementation
Bottom Up Dynamic Programming is used. Since we need to maximize total price and minimize total weight, 2 DP memoization are used.
Ideally, N x V arrays are needed for DP memoization where N is number of product and V is max tote volume. However, there is a need to reduce DP memoization space
from N x V to V (only concern to the previous product number constraint) since the immediate allocation of N*W triggered stack overflow in C++. 
So, there is a need for another DP memoization for saving last picked product and adjacency list for backtracking. The adjcency list add edges from i th product to
i-x product with weight edge possible volume state of when choosing i-x th product.
Time complexity is O(NV) and space complexity is O(NV) since adjacency list add edges from i th product to i-x th product with different volume.
It might be convenient to use Python since it can allocate large stack and heap memory. However, Python is extremely slow compared to C++.
So, C++11 is choosen for implementation.

#include<iostream>
#include<vector>
#include<stdlib.h>
#include<pthread.h>
#include <chrono>

struct Args
{
    int first_i;
    int first_j;
    int last_i;
    int last_j;
    std::vector<std::vector<int>>* matrix;

    Args()
        : first_i(0)
        , first_j(0)
        , last_i(0)
        , last_j(0)
        , matrix(nullptr)
    {}
};

void* countSum(void* args);


class ParallelMatrix
{
private:
    int m;
    int n;
    
    std::vector<std::vector<int>> matrix;
public:
    ParallelMatrix(int m = 1, int n = 1)
        : m(m)
        , n(n)
        , matrix(std::vector<std::vector<int>>(m, std::vector<int>(n)))
    {}

    int sum();
    int sumParallel(int threadsNum);
    void init();
};


void measureTime(ParallelMatrix& matrix, int arg, int (ParallelMatrix::*func)(int));

void measureTime(ParallelMatrix& matrix, int (ParallelMatrix::*func)());
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<pthread.h>
#include <chrono>

// structer for thread arguments
struct Args
{
    int first_i;
    int first_j;
    int last_i;
    int last_j;
    std::vector<std::vector<int>>* matrix;

    // constructer
    Args()
        : first_i(0)
        , first_j(0)
        , last_i(0)
        , last_j(0)
        , matrix(nullptr)
    {}
};

void* countSum(void* args);

// matrix data structure
class ParallelMatrix
{
private:
    int m;
    int n;
    
    std::vector<std::vector<int>> matrix;
public:

    // constructer
    ParallelMatrix(int m = 1, int n = 1)
        : m(m)
        , n(n)
        , matrix(std::vector<std::vector<int>>(m, std::vector<int>(n)))
    {}

    // calculates sum of all cells in the matrix
    int sum();

    // calculates the sum of all cells using threads 
    int sumParallel(int threadsNum);

    // initializes all the cells with random integers
    void init();
};

// measures the times of sumParallel
void measureTime(ParallelMatrix& matrix, int arg, int (ParallelMatrix::*func)(int));

// measures the times of sum
void measureTime(ParallelMatrix& matrix, int (ParallelMatrix::*func)());
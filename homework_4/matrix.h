#include<iostream>
#include<vector>
#include <stdlib.h>

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
    void sumParallel();
    void init();
};

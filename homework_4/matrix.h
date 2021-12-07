#include<iostream>
#include<vector>

class ParallelMatrix
{
private:
    std::vector<std::vector<int>> matrix;
public:
    ParallelMatrix()
    {}

    void sum();
    void sumParallel();
    void init();
};

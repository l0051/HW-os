#include "matrix.h"

int ParallelMatrix::sum()
{
    int sum;
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            sum += matrix[i][j];
        }
    }
    return sum;
}

void ParallelMatrix::sumParallel()
{

}

void ParallelMatrix::init()
{
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            matrix[i][j] = rand();
        }
    }
}
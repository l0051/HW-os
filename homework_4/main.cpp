#include "matrix.h"

int main()
{
    ParallelMatrix matrix(500, 600);
    matrix.init();
    //std::cout << matrix.sumParallel(100) << " " << matrix.sum() << std::endl;

    measureTime(matrix, &ParallelMatrix::sum);
    
    measureTime(matrix, 4, &ParallelMatrix::sumParallel);
    measureTime(matrix, 8, &ParallelMatrix::sumParallel);
    measureTime(matrix, 16, &ParallelMatrix::sumParallel);
    measureTime(matrix, 100, &ParallelMatrix::sumParallel);
    
    std::cout << std::endl;

}

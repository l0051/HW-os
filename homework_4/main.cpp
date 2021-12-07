#include "matrix.h"

int main()
{
    // creates a matrix with size 500 and 600
    ParallelMatrix matrix(500, 600);

    // initializes that matrix with random integers
    matrix.init();
    //std::cout << matrix.sumParallel(100) << " " << matrix.sum() << std::endl;

    // measures the times of sum for matrix
    measureTime(matrix, &ParallelMatrix::sum);
    
    // measures the times of sumParallel for matrix with 4, 8, 16, 100 threads
    measureTime(matrix, 4, &ParallelMatrix::sumParallel);
    measureTime(matrix, 8, &ParallelMatrix::sumParallel);
    measureTime(matrix, 16, &ParallelMatrix::sumParallel);
    measureTime(matrix, 100, &ParallelMatrix::sumParallel);
    
    std::cout << std::endl;

}

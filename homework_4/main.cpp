#include "matrix.h"

int main()
{
    ParallelMatrix x = ParallelMatrix(5, 6);
    x.init();
    std::cout << x.sumParallel(5) << " " << x.sum() << std::endl;

}
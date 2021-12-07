#include "matrix.h"

int main()
{
    ParallelMatrix x = ParallelMatrix(5, 6);
    x.init();
    std::cout << x.sumParallel(10) << " " << x.sum();

}

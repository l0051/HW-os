#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

int main()
{
    // make array with random numbers
    size_t n;
    std::cin >> n;
    std::vector<int> v(n);
    srand(time(NULL));
    for (size_t i = 0; i < n; ++i)
    {
        v[i] = rand() % 10000;
    }

    // get number of paralell workers
    size_t m;
    std::cin >> m;

    
}
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cerrno>

int main()
{
    // make array with random numbers
    size_t n;
    std::cin >> n;
    int* array = new int[n];
    srand(time(NULL));
    for (size_t i = 0; i < n; ++i)
    {
        array[i] = rand() % 10000;
    }

    // get number of paralell workers
    size_t m;
    std::cin >> m;

    for (size_t i = 0; i < m; ++i)
    {
        int pipefd_index[2];
        int pipefd_sum[2];
        int res_index = pipe(pipefd_index);
        int res_sum = pipe(pipefd_sum);

        // error handling
        if (res_index < 0)
        {
            std::cerr << "Error while creating pipe " << errno <<std::endl;
            exit(errno);
        }
        if (res_sum < 0)
        {
            std::cerr << "Error while creating pipe " << errno <<std::endl;
            exit(errno);
        }
        
        // clone the process
        pid_t pid = fork();

        // error handling
        if (pid < 0)
        {
            std::cerr << "Error while making fork " << errno <<std::endl;
            exit(errno);
        }

        // child process
        if (pid == 0)
        {

        }

        // parent process
        if (pid > 0)
        {
            
        }
    }
}
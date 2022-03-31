#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cerrno>
#include <sys/wait.h>

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

    int pipefd_sum[2];
    int res_sum = pipe(pipefd_sum);

    // error handling
    if (res_sum < 0)
    {
        std::cerr << "Error while creating pipe " << errno <<std::endl;
        exit(errno);
    }

    for (size_t i = 0; i < m; ++i)
    {
        int pipefd_index[2];
        int res_index = pipe(pipefd_index);

        // error handling
        if (res_index < 0)
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
            close(pipefd_index[0]);
            close(pipefd_sum[1]);
            size_t * indexes = new size_t[2];
            void * indexes_void = (void *) &indexes;
            read(pipefd_index[1], indexes_void, 2 * sizeof(size_t));
            
            long long int sum = 0;
            for (size_t j = indexes[0]; j < indexes[1]; ++j)
            {
                sum += array[j];
            }
            std::cout << sum << " ";
            void * sum_void = (void *) sum;
            write(pipefd_sum[0], sum_void, sizeof(long long int)); 
            exit(0);
        }

        // parent process
        if (pid > 0)
        {
            close(pipefd_index[1]);
            close(pipefd_sum[0]);
            close(pipefd_sum[1]); 
            size_t * indexes = new size_t[2];
            indexes[0] = (n / m) * i;
            indexes[1] = (i + 1 == m) ? n : ((n / m) * (i + 1)); // last end
            void * indexes_void = (void *) &indexes;
            write(pipefd_index[0], indexes_void, 2 * sizeof(size_t));
        }
    }

    wait(NULL);

    long long int sum = 0;
    for (size_t i = 0; i < m; ++i)
    {
        close(pipefd_sum[0]);
        void * sum_void;
        read(pipefd_sum[1], sum_void, sizeof(long long int));
        long long int * sub_sum = (long long *) sum_void;
        sum += *sub_sum;
    }

    std::cout << "\n" << sum << std::endl;
}
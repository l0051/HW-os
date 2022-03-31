#include <iostream>
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
    
    // variable for error handling
    int res;

    // make a pipe for subsum
    int pipefd_sum[2];    
    res = pipe(pipefd_sum);

    // error handling
    if (res < 0)
    {
        std::cerr << "Error while creating pipe " << errno <<std::endl;
        exit(errno);
    }

    // make m child process
    for (size_t i = 0; i < m; ++i)
    {
        // make a pipe for i, j
        int pipefd_index[2];
        res = pipe(pipefd_index);

        // error handling
        if (res < 0)
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

            // for reading i, j
            void * indexes_void;
            // read i, j
            res = read(pipefd_index[1], indexes_void, 2 * sizeof(size_t));
            // cast to size_t  *
            size_t * indexes = (size_t *) indexes_void;

            // error handling
            if (res < 0)
            {
                std::cerr << "Error while reading " << errno <<std::endl;
                exit(errno);
            }

            // count subsum
            long long int sum = 0;
            for (size_t j = indexes[0]; j < indexes[1]; ++j)
            {
                sum += array[j];
            }
            std::cout << sum << " ";
            // cast to void *
            void * sum_void = (void *) sum;
            // write subsum
            res = write(pipefd_sum[0], sum_void, sizeof(long long int)); 
            
            // error handling
            if (res < 0)
            {
                std::cerr << "Error while writing " << errno <<std::endl;
                exit(errno);
            }
            exit(0);
        }

        // parent process
        if (pid > 0)
        {
            close(pipefd_index[1]);
            close(pipefd_sum[0]);
            close(pipefd_sum[1]); 

            // for writing i, j
            size_t * indexes = new size_t[2];
            // count i, j
            indexes[0] = (n / m) * i;
            indexes[1] = (i + 1 == m) ? n : ((n / m) * (i + 1)); // last end
            // cast to void *
            void * indexes_void = (void *) &indexes;

            // write indexes
            res = write(pipefd_index[0], indexes_void, 2 * sizeof(size_t));
            
            // error handling
            if (res < 0)
            {
                std::cerr << "Error while writing " << errno <<std::endl;
                exit(errno);
            }

            // ????????????????????????/
            // wait the end of child
            wait(NULL);
        }
    }

    close(pipefd_sum[0]);
    // for subsums
    void * sum_void;
    // read subsums
    res = read(pipefd_sum[1], sum_void, sizeof(long long int) * m);

    // error handling
    if (res < 0)
    {
        std::cerr << "Error while reading " << errno <<std::endl;
        exit(errno);
    }

    // count sum
    long long int sum = 0;
    long long int * sub_sum = (long long *) sum_void;
    for (int i = 0; i < m; ++i)
    {
        sum += sub_sum[i];
    }

    // print sum
    std::cout << "\n" << sum << std::endl;

    // allocate memory
    delete [] array;
}
#include "matrix.h"

void measureTime(ParallelMatrix& matrix, int arg, int (ParallelMatrix::*func)(int))
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    // gets start time
    auto t1 = high_resolution_clock::now();

    // runs function
    (matrix.*func)(arg);

    // gets end time
    auto t2 = high_resolution_clock::now();

    // count duration of function in ms
    duration<double, std::milli> msDouble = t2 - t1;

    // print duration
    std::cout << msDouble.count() << "ms" << " ";
    
}

void measureTime(ParallelMatrix& matrix, int (ParallelMatrix::*func)())
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    // gets start time
    auto t1 = high_resolution_clock::now();

    // runs function
    (matrix.*func)();

    // gets end time
    auto t2 = high_resolution_clock::now();

    // count duration of function in ms
    duration<double, std::milli> msDouble = t2 - t1;

    // print duration
    std::cout << msDouble.count() << "ms" << " ";
}

void* countSum(void* args)
{
    Args* submatrixArg = (Args*) args;
    int submatrixSum = 0;
    for (int i = submatrixArg->first_i; i < submatrixArg->last_i; ++i)
    {
        for (int j = submatrixArg->first_j; j < submatrixArg->last_j; ++j)
        {
            submatrixSum += (*submatrixArg->matrix)[i][j];
        }
    }
    return (void*) (new int(submatrixSum));
}

int ParallelMatrix::sum()
{
    int sum;

    // runs throught the matrix element and added them with the amount
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            sum += matrix[i][j];
        }
    }
    return sum;
}

int ParallelMatrix::sumParallel(int threadsNum)
{
    // handles error
    if (threadsNum > m * n)
    {
        std::cerr << "Could not have more threads than elements in matrix" << std::endl;
        exit(0);
    }

    int sum = 0;

    // creates an array of pthread_t
    pthread_t* threads = new pthread_t[threadsNum];

    // creates an array of arguments to pass to the thread
    Args* args = new Args[threadsNum]; 

    if (threadsNum <= m)
    {        
        // divides matrix into submatrix
        for (int i = 0, k = 0; i < m && k < threadsNum; i += m / threadsNum, ++k)
        {
            // creates an argument
            args[k].first_i = i;
            args[k].last_i = i + m / threadsNum;
            if (k + 1 == threadsNum)
		        args[k].last_i = m;
		    args[k].first_j = 0;
            args[k].last_j = n;
            args[k].matrix = &matrix;

            // creates a thread
            int result = pthread_create(&threads[k], NULL, countSum, &args[k]);

            // handles error
		    if(result != 0)
		    {
                std::cerr << "Error while creting thread" << std::endl;
			    exit(result);
		    }
        }
    }
    else if (threadsNum <= n)
    {
        // divides matrix into submatrix
        for (int i = 0, k = 0; i < n && k < threadsNum; i += n / threadsNum, ++k)
        {
            // creates an argument
            args[k].first_j = i;
            args[k].last_j = i + n / threadsNum;
            if (k + 1 == threadsNum)
		        args[k].last_j = n;
		    args[k].first_i = 0;
            args[k].last_i = m;
            args[k].matrix = &matrix;

            // creates a thread
            int result = pthread_create(&threads[k], NULL, countSum, &args[k]);
		    
            // handles error
            if(result != 0)
		    {
                std::cerr << "Error while creting thread" << std::endl;
			    exit(result);
		    }
        }
    }
    else
    {
        if (threadsNum == 1)
        {
            // creates an argument
            args[0].first_j = 0;
            args[0].last_j = n;
		    args[0].first_i = 0;
            args[0].last_i = m;
            args[0].matrix = &matrix;

            // creates a thread
            int result = pthread_create(&threads[0], NULL, countSum, &args[0]);
		                
            // handles error
            if(result != 0)
		    {
                std::cerr << "Error while creting thread" << std::endl;
			    exit(result);
		    }
        }
        else
        {
            // divides matrix into submatrix
            for (int i = 0, j = 0, k = 0; k < threadsNum && i < m && j < n; ++i, ++j, ++k)
            {
                if (k + 1 != threadsNum)
                {
                    // creates an argument
    		        args[k].first_i = i;
                    args[k].last_i = i + 1;
                    args[k].first_j = j;
                    args[k].last_j = j + 1;
                    args[k].matrix = &matrix;

                    // creates a thread
                    int result = pthread_create(&threads[k], NULL, countSum, &args[k]);
		            
                    // handles error
                    if(result != 0)
		            {
                        std::cerr << "Error while creting thread" << std::endl;
			            exit(result);
		            }
                }
                else
                {
                    // creates an argument
                    args[k].first_i = i;
                    args[k].last_i = m;
                    args[k].first_j = j;
                    args[k].last_j = n;
                    args[k].matrix = &matrix;

                    // creates a thread
                    int result = pthread_create(&threads[k], NULL, countSum, &args[k]);
		            
                    // handles error
                    if(result != 0)
		            {
                        std::cerr << "Error while creting thread" << std::endl;
			            exit(result);
		            }
                }
            }
        }
        
    }

    // memory for storing the sum of the submatrix 
    void* submatrixSum;

    // runs throught the threads
    for (int i = 0; i < threadsNum; ++i)
    {
        // joins thread
        int joined = pthread_join(threads[i], &submatrixSum);
        
        // handles error
        if (joined != 0)
        {
            std::cerr << "Error while joining thread" << std::endl;
            exit(joined);
        }

        // converts submatrixSum to int* and add value to summ
        int* submatrixSumInt = (int*) submatrixSum;
        sum += *submatrixSumInt;

        // free allocated memory
        delete submatrixSumInt;
    }

    // free allocated memory
    delete [] threads;
    delete [] args;

    return sum;
}

void ParallelMatrix::init()
{
    // runs throught the matrix element and initialized them by random integers
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            srand((unsigned) time(0));
            matrix[i][j] = rand();
        }
    }
}
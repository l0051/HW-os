#include "matrix.h"

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
    if (threadsNum > m * n)
    {
        std::cerr << "Could not have more threads than elements in matrix" << std::endl;
        exit(0);
    }

    int sum = 0;
    pthread_t* threads = new pthread_t[threadsNum];
    Args* args = new Args[threadsNum]; 
    if (threadsNum <= m)
    {        
        for (int i = 0, k = 0; i <= m && k < threadsNum; i += m / threadsNum, ++k)
        {
            args[k].first_i = i;
            args[k].last_i = i + m / threadsNum;
            if (k + 1 == threadsNum)
		        args[k].last_i = m;
		    args[k].first_j = 0;
            args[k].last_j = n;
            int result = pthread_create(&threads[k], NULL, countSum, &args[k]);
		    if(result != 0)
		    {
			    exit(result);
		    }
        }
    }
    else if (threadsNum <= n)
    {
        for (int i = 0, k = 0; i <= n && k < threadsNum; i += n / threadsNum, ++k)
        {
            args[k].first_j = i;
            args[k].last_j = i + n / threadsNum;
            if (k + 1 == threadsNum)
		        args[k].last_j = n;
		    args[k].first_i = 0;
            args[k].last_i = m;
            int result = pthread_create(&threads[k], NULL, countSum, &args[k]);
		    if(result != 0)
		    {
			    exit(result);
		    }
        }
    }
    else
    {
        if (threadsNum == 1)
        {
            args[0].first_j = 0;
            args[0].last_j = n;
		    args[0].first_i = 0;
            args[0].last_i = m;
            int result = pthread_create(&threads[0], NULL, countSum, &args[0]);
		    if(result != 0)
		    {
			    exit(result);
		    }
        }
        else
        {
            for (int i = 0, j = 0, k = 0; k < threadsNum && i < m && j < n; ++i, ++j, ++k)
            {
                if (k + 1 != threadsNum)
                {
    		        args[k].first_i = i;
                    args[k].last_i = i + 1;
                    args[k].first_j = j;
                    args[k].last_j = j + 1;
                    int result = pthread_create(&threads[k], NULL, countSum, &args[k]);
		            if(result != 0)
		            {
			            exit(result);
		            }
                }
                else
                {
                    args[k].first_i = i;
                    args[k].last_i = m;
                    args[k].first_j = j;
                    args[k].last_j = n;
                    int result = pthread_create(&threads[k], NULL, countSum, &args[k]);
		            if(result != 0)
		            {
			            exit(result);
		            }
                }
            }
        }
        
    }
    void* submatrixSum;
    for (int i = 0; i <= threadsNum; ++i)
    {
        int joined = pthread_join(threads[i], &submatrixSum);
        if (joined != 0)
        {
            exit(joined);
        }
        sum += *((int *) submatrixSum);
    }
    delete [] threads;
    delete [] args;
    return sum;
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
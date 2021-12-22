#include <iostream>
#include <queue>
#include <pthread.h>

void* function(void* args_void);


struct Args
{
    int* capacity;
    std::queue<void(*)(void*)>* functions;
    std::queue<void*>* arguments;
    pthread_mutex_t** mutex;
    pthread_cond_t** cond;

    Args(int* capacity, std::queue<void(*)(void*)>* functions, std::queue<void*>* arguments, pthread_mutex_t** mutex, pthread_cond_t** cond)
        : capacity(capacity)
        , functions(functions)
        , arguments(arguments)
        , mutex(mutex)
        , cond(cond)
    {}
};

class ParallelScheduler
{
public:
    ParallelScheduler(int capacity)
        : capacity(capacity)
        , threads(new pthread_t[capacity])
        , mutex(new pthread_mutex_t)
        , cond(new pthread_cond_t)
    {
        int mutex_init = pthread_mutex_init(mutex, NULL);
        if (mutex_init != 0)
        {
            std::cerr << "Error while initializing a mutex object" << std::endl;
			exit(mutex_init);
        }

        int cond_init = pthread_cond_init(cond, NULL);
        if (cond_init != 0)
        {
            std::cerr << "Error while initializing a condition variable" << std::endl;
			exit(cond_init);
        }

        Args* args = new Args(&capacity, &functions, &arguments, &mutex, &cond);
        void* args_void = (void*) args;
        for (int i = 0; i < capacity; ++i)
        {
            int result = pthread_create(&threads[i], NULL, function, args_void);
            if (result != 0)
            {
                std::cerr << "Error while creating thread" << std::endl;
                exit(result);
            }
        }
    }

    ~ParallelScheduler()
    {
        int cond_destroy = pthread_cond_destroy(cond);
        if (cond_destroy != 0)
        {
            std::cerr << "Error while destroying a conditional viriable state" << std::endl;
			exit(cond_destroy);
        }

        int mutex_destroy = pthread_mutex_destroy(mutex);
        if (mutex_destroy != 0)
        {
            std::cerr << "Error while destroying a mutex object" << std::endl;
			exit(mutex_destroy);
        }
        
        delete cond;
        delete mutex;
        delete [] threads;
    }

    void run(void (*start_routine)(void*), void* arg);

private:
    int capacity;
    pthread_t* threads;
    std::queue<void(*)(void*)> functions;
    std::queue<void*> arguments;
    
    pthread_mutex_t* mutex;
    pthread_cond_t* cond;
};

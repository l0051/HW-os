#include <iostream>
#include <queue>
#include <pthread.h>

struct Args
{
    int* capacity;
    std::queue<void(*)(void*)>* functions;
    std::queue<void*>* arguments;
    pthread_mutex_t** mutex;
    pthread_cond_t** cond;

    Args()
        : capacity(nullptr)
        , functions(nullptr)
        , arguments(nullptr)
        , mutex(nullptr)
        , cond(nullptr)
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
    
    //void* function(void*);
    void smth();
};

void* function(void* args_void);
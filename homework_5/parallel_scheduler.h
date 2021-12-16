#include <iostream>
#include <queue>
#include <pthread.h>



class ParallelScheduler
{
public:
    ParallelScheduler(int capacity)
        : capacity(capacity)
        , threads(new pthread_t[capacity])
    {}

    ~ParallelScheduler()
    {
        delete cond;
        delete mutex;
        delete threads;
    }

    void run(void (*start_routine)(void*), void* arg);

private:
    int capacity;
    pthread_t* threads;
    std::queue<void(*)(void*)> functions;
    
    pthread_mutex_t*  mutex = new pthread_mutex_t;
    pthread_cond_t* cond = new pthread_cond_t;

    void function(void (*start_routine)(void*), void* arg);

};
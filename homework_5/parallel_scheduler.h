#include <iostream>
#include <queue>
#include <functional>
#include <pthread.h>

class ParallelScheduler
{
public:
    ParallelScheduler(int capacity)
        : capacity(capacity)
        , threads(new pthread_t[capacity])
    {
        //for (int i = 0; i < capacity; ++i)
        //{
            //pthread_create(&threads[i], NULL, ?, ?);
        //}
    }

    void run(void(*start_routine)(void*), void*);

private:
    int capacity;
    pthread_t* threads;
    std::queue<std::function<void(void*)>> functions;
    
};
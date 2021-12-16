#include "parallel_scheduler.h"

void* ParallelScheduler::function(void*)
{
    // mutex ...
    pthread_mutex_lock(mutex);
    pthread_cond_wait(cond, mutex);
    

    //....

    

    //....
    functions.front()(arguments.front());
    functions.pop();
    arguments.pop();
    //....
    
    pthread_mutex_unlock(mutex);

    // ...
    return nullptr;
}

void ParallelScheduler::run(void (*start_routine)(void*), void* arg)
{
    functions.push(start_routine);
    arguments.push(arg);
    function();
}
#include "parallel_scheduler.h"

void ParallelScheduler::function()
{
    // mutex ...
    pthread_mutex_lock(mutex);
    pthread_cond_wait(cond, mutex);
    
    //....
    functions.front()(arguments.front());
    //....
    
    pthread_mutex_unlock(mutex);

    // ...
}

void ParallelScheduler::run(void (*start_routine)(void*), void* arg)
{
    functions.push(start_routine);
    arguments.push(arg);
    function();
}
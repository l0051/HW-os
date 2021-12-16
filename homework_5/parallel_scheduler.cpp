#include "parallel_scheduler.h"

void ParallelScheduler::function(void (*start_routine)(void*), void* arg)
{
    // mutex ...
    pthread_mutex_lock(mutex);
    pthread_cond_wait(cond, mutex);
    
    //....
    functions.front()(arg);//arg dismatch
    //....
    
    pthread_mutex_unlock(mutex);

    // ...
}

void ParallelScheduler::run(void (*start_routine)(void*), void* arg)
{
    functions.push(start_routine);
    function(start_routine, arg);
}
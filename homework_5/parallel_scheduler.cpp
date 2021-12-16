#include "parallel_scheduler.h"

void ParallelScheduler::function(void (*start_routine)(void*), void* arg)
{
    // mutex ...

    

    // ...
}

void ParallelScheduler::run(void (*start_routine)(void*), void* arg)
{
    functions.push(start_routine);
    function(start_routine, arg);
}
#include "parallel_scheduler.h"

void ParallelScheduler::smth()
{
    Args* args = new Args;
    args->capacity = &capacity;
    args->arguments = &arguments;
    args->mutex = &mutex;
    args->cond = &cond;
    args->functions = &functions;
    void* args_void = (void*) args;
    for (int i = 0; i < capacity; ++i)
    {
        pthread_create(&threads[i], NULL, function, args_void);
    }
}

void* function(void* args_void)
{
    Args* args = (Args*) args_void;
    // mutex ...
    pthread_mutex_lock(*(args->mutex));
    if ((*(args->functions)).size() > *(args->capacity))
    {
        pthread_cond_wait(*(args->cond), *(args->mutex));
    }

    //....

    

    //....
    (*(args->functions)).front()((*(args->arguments)).front());
    (*(args->functions)).pop();
    (*(args->arguments)).pop();

    //
    if ((*(args->functions)).size() <= *(args->capacity))
    {
        pthread_cond_signal(*(args->cond));
    }
    //....
    
    pthread_mutex_unlock(*(args->mutex));

    // ...
    return nullptr;
}

void ParallelScheduler::run(void (*start_routine)(void*), void* arg)
{
    functions.push(start_routine);
    arguments.push(arg);
    smth();
}
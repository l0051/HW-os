#include "parallel_scheduler.h"

void* function(void* args_void)
{
    while (true)
    {
        Args* args = (Args*) args_void;

        while (!(*(args->functions)).empty())
        {
            pthread_mutex_lock(*(args->mutex));    
            
            auto current_func = (*(args->functions)).front();
            auto current_arg = (*(args->arguments)).front();

            (*(args->functions)).pop();
            (*(args->arguments)).pop();
            
            pthread_mutex_unlock(*(args->mutex));
            current_func(current_arg);
        }
        
        while ((*(args->functions)).empty())
        {
            
            pthread_mutex_lock(*(args->mutex));
            pthread_cond_wait(*(args->cond), *(args->mutex));
            pthread_mutex_unlock(*(args->mutex));
        }
    }
    return nullptr;
}

void ParallelScheduler::run(void (*start_routine)(void*), void* arg)
{
    pthread_mutex_lock(mutex);
    functions.push(start_routine);
    arguments.push(arg);
    pthread_cond_signal(cond);
    pthread_mutex_unlock(mutex);

}
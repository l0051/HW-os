#include "parallel_scheduler.h"

void* function(void* args_void)
{
    while (true)
    {
        int result;

        Args* args = (Args*) args_void;
        while (!(*(args->functions)).empty())
        {
            result = pthread_mutex_lock(*(args->mutex));    
            if (result != 0)
            {
                std::cerr << "Error while locking" << std::endl;
                exit(result);
            }


            auto current_func = (*(args->functions)).front();
            auto current_arg = (*(args->arguments)).front();

            (*(args->functions)).pop();
            (*(args->arguments)).pop();
            
            result = pthread_mutex_unlock(*(args->mutex));
            if (result != 0)
            {
                std::cerr << "Error while unlocking" << std::endl;
                exit(result);
            }
            current_func(current_arg);
        }
        
        while ((*(args->functions)).empty())
        {
            
            result = pthread_mutex_lock(*(args->mutex));
            if (result != 0)
            {
                std::cerr << "Error while locking" << std::endl;
                exit(result);
            }
            result = pthread_cond_wait(*(args->cond), *(args->mutex));
            if (result != 0)
            {
                std::cerr << "Error while start waiting" << std::endl;
                exit(result);
            }
            result = pthread_mutex_unlock(*(args->mutex));
            if (result != 0)
            {
                std::cerr << "Error while unlocking" << std::endl;
                exit(result);
            }
        }
    }
    return nullptr;
}

void ParallelScheduler::run(void (*start_routine)(void*), void* arg)
{
    int result;
    result = pthread_mutex_lock(mutex);
    if (result != 0)
    {
        std::cerr << "Error while locking" << std::endl;
        exit(result);
    }
    functions.push(start_routine);
    arguments.push(arg);
    result = pthread_cond_signal(cond);
    if (result != 0)
    {
        std::cerr << "Error while signaling" << std::endl;
        exit(result);
    }
    result = pthread_mutex_unlock(mutex);
    if (result != 0)
    {
        std::cerr << "Error while unlocking" << std::endl;
        exit(result);
    }

}
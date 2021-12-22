#include "parallel_scheduler.h"

void* function(void* args_void)
{
    while (true)
    {
        // for errors
        int result;

        Args* args = (Args*) args_void;
        while (!(*(args->functions)).empty())
        {
            // locks thread
            result = pthread_mutex_lock(*(args->mutex));    
            
            // handles error
            if (result != 0)
            {
                std::cerr << "Error while locking" << std::endl;
                exit(result);
            }

            // function and argument
            auto current_func = (*(args->functions)).front();
            auto current_arg = (*(args->arguments)).front();

            // removes function from queue
            (*(args->functions)).pop();
            (*(args->arguments)).pop();
            
            // unlocks thread
            result = pthread_mutex_unlock(*(args->mutex));
            
            // handles error
            if (result != 0)
            {
                std::cerr << "Error while unlocking" << std::endl;
                exit(result);
            }

            // runs function
            current_func(current_arg);
        }
        
        while ((*(args->functions)).empty())
        {
            // locks thread
            result = pthread_mutex_lock(*(args->mutex));
            
            // handles error
            if (result != 0)
            {
                std::cerr << "Error while locking" << std::endl;
                exit(result);
            }

            // waits for signal
            result = pthread_cond_wait(*(args->cond), *(args->mutex));
            
            // handles error
            if (result != 0)
            {
                std::cerr << "Error while start waiting" << std::endl;
                exit(result);
            }

            // unlocks thread
            result = pthread_mutex_unlock(*(args->mutex));

            // handles error
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
    // for errors
    int result;

    // locks thread
    result = pthread_mutex_lock(mutex);

    // handles error
    if (result != 0)
    {
        std::cerr << "Error while locking" << std::endl;
        exit(result);
    }

    // adds function to the queue
    functions.push(start_routine);
    arguments.push(arg);

    // gives a signal
    result = pthread_cond_signal(cond);

    // handles error
    if (result != 0)
    {
        std::cerr << "Error while signaling" << std::endl;
        exit(result);
    }

    // locks thread
    result = pthread_mutex_unlock(mutex);

    // handles error
    if (result != 0)
    {
        std::cerr << "Error while unlocking" << std::endl;
        exit(result);
    }

}
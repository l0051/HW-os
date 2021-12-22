#include <iostream>
#include <queue>
#include <pthread.h>

void* function(void* args_void);

// structer for thread arguments
struct Args
{
    int* capacity;
    std::queue<void(*)(void*)>* functions;
    std::queue<void*>* arguments;
    pthread_mutex_t** mutex;
    pthread_cond_t** cond;

    // constructer
    Args(int* capacity, std::queue<void(*)(void*)>* functions, std::queue<void*>* arguments, pthread_mutex_t** mutex, pthread_cond_t** cond)
        : capacity(capacity)
        , functions(functions)
        , arguments(arguments)
        , mutex(mutex)
        , cond(cond)
    {}
};

// module
class ParallelScheduler
{
public:
    // constructer
    ParallelScheduler(int capacity)
        : capacity(capacity)
        , threads(new pthread_t[capacity])
        , mutex(new pthread_mutex_t)
        , cond(new pthread_cond_t)
    {
        // initializes mutex
        int mutex_init = pthread_mutex_init(mutex, NULL);
        
        // handles error
        if (mutex_init != 0)
        {
            std::cerr << "Error while initializing a mutex object" << std::endl;
			exit(mutex_init);
        }

        // initializes conditional variable
        int cond_init = pthread_cond_init(cond, NULL);
        
        // handles error
        if (cond_init != 0)
        {
            std::cerr << "Error while initializing a condition variable" << std::endl;
			exit(cond_init);
        }

        // creates arguments for threads
        Args* args = new Args(&capacity, &functions, &arguments, &mutex, &cond);
        void* args_void = (void*) args;
        for (int i = 0; i < capacity; ++i)
        {
            // creates a thread
            int result = pthread_create(&threads[i], NULL, function, args_void);
            
            // handles error
            if (result != 0)
            {
                std::cerr << "Error while creating thread" << std::endl;
                exit(result);
            }
        }

        // free allocated memory
        delete args;
    }

    ~ParallelScheduler()
    {
        // destroys conditional variable
        int cond_destroy = pthread_cond_destroy(cond);
        
        // handles error
        if (cond_destroy != 0)
        {
            std::cerr << "Error while destroying a conditional viriable state" << std::endl;
			exit(cond_destroy);
        }

        // destroys mutex
        int mutex_destroy = pthread_mutex_destroy(mutex);
        
        // handles error
        if (mutex_destroy != 0)
        {
            std::cerr << "Error while destroying a mutex object" << std::endl;
			exit(mutex_destroy);
        }
        
        for (int i = 0; i < capacity; ++i)
        {
            // joins thread
            int joined = pthread_join(threads[i], NULL);

            // handles error
            if (joined != 0)
            {
                std::cerr << "Error while joining thread" << std::endl;
                exit(joined);
            }
        }
        
        // free allocated memory
        delete cond;
        delete mutex;
        delete [] threads;
    }

    // calls funtions
    void run(void (*start_routine)(void*), void* arg);

private:
    int capacity;
    pthread_t* threads;

    // queue of functions
    std::queue<void(*)(void*)> functions;

    // queue of arguments
    std::queue<void*> arguments;
    
    pthread_mutex_t* mutex;
    pthread_cond_t* cond;
};

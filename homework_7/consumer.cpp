#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <climits>
#include <cerrno>

const size_t BUFFER_SIZE = getpagesize();

int main()
{
    // (create and) open shared memory object
    int shared_fd = shm_open("/prod-cons-buffer", O_CREAT | O_RDONLY, S_IRGRP);
    
    // handle error
    if (shared_fd < 0)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}

    // change size of shared memory
    int er = ftruncate(shared_fd, getpagesize());

    // handle error
    if (er < 0)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}

    // creates a new mapping in the virtual address space of the calling process
    void* shared_pointer_void = mmap(NULL, BUFFER_SIZE, PROT_WRITE, PROT_WRITE, shared_fd, 0);
    
    // handle error
    if (*(int *)shared_pointer_void < 0)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}

    // semaphore for reading
    size_t* shared_fullness = (size_t*) shared_pointer_void;
    //*shared_fullness = 0;
    
    char* shared_pointer = (char*) shared_pointer_void;
    shared_pointer += CHAR_BIT * sizeof(size_t);

    mmap(NULL, BUFFER_SIZE, PROT_WRITE, PROT_WRITE, shared_fd, 0);

    sem_t* sem_shared = new sem_t;

    sem_t* sem_full = sem_open("/full", O_CREAT, 777, BUFFER_SIZE);
    
    // handle error
    if (sem_full == SEM_FAILED)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}

    sem_t* sem_empty = sem_open("/empty", O_CREAT, 777, 0);

    // handle error
    if (sem_empty == SEM_FAILED)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}

    er = sem_init(sem_shared, 0, 1);

    // handle error
    if (er < 0)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}

    while (true)
    {
        sem_wait(sem_empty);
        sem_wait(sem_shared);
        
        shared_pointer[*shared_fullness - 1] = '/0';
        --*shared_fullness;

        sem_post(sem_shared);
        sem_post(sem_full);
    }

    // destroy unnamed semaphore
    sem_destroy(sem_shared);

    // close named semaphore
    sem_close(sem_empty);
    sem_close(sem_full);

    // free allocated memory
    delete sem_shared;

    // removes the named semaphore referred to by name
    sem_unlink("/empty");
    sem_unlink("/full");

    // remove an object previously created by shm_open()
    shm_unlink("/prod-cons-buffer");
}
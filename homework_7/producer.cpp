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
    int shared_fd = shm_open("/prod-cons-buffer", O_CREAT | O_WRONLY, S_IWGRP);

    if (shared_fd < 0)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}

    int er = ftruncate(shared_fd, getpagesize());

    if (er < 0)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}
    
    void* shared_pointer_void = mmap(NULL, BUFFER_SIZE, PROT_WRITE, PROT_WRITE, shared_fd, 0);

    if (*(int *)shared_pointer_void < 0)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}

    size_t* shared_fullness = (size_t*) shared_pointer_void;
    //*shared_fullness = 0;
    char* shared_pointer = (char*) shared_pointer_void;
    shared_pointer += CHAR_BIT * sizeof(size_t);
    sem_t* sem_shared = new sem_t;

    sem_t* sem_full = sem_open("/full", O_CREAT, 777, BUFFER_SIZE);

    if (sem_full == SEM_FAILED)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}
    
    sem_t* sem_empty = sem_open("/empty", O_CREAT, 777, 0);
    
    if (sem_empty == SEM_FAILED)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}

    er = sem_init(sem_shared, 0, 1);

    if (er < 0)
	{
		std::cerr << "Error " << std::endl;
		exit(errno);
	}

    while (true)
    {
        sem_wait(sem_full);
        sem_wait(sem_shared);
        
        shared_pointer[*shared_fullness] = 'a';
        +*shared_fullness;
        sem_post(sem_shared);
        sem_post(sem_empty);
    }
    
    sem_destroy(sem_shared);
    sem_close(sem_empty);
    sem_close(sem_full);
    delete sem_shared;
    sem_unlink("/empty");
    sem_unlink("/full");
    shm_unlink("/prod-cons-buffer");
}
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <climits>

const size_t BUFFER_SIZE = getpagesize();

int main()
{
    int shared_fd = shm_open("/prod-cons-buffer", O_CREAT | O_RDONLY, S_IRGRP);
    
    ftruncate(shared_fd, getpagesize());

    void* shared_pointer_void = mmap(NULL, BUFFER_SIZE, PROT_WRITE, PROT_WRITE, shared_fd, 0);
    
    size_t* shared_fullness = (size_t*) shared_pointer_void;
    //*shared_fullness = 0;
    char* shared_pointer = (char*) shared_pointer_void;
    shared_pointer += CHAR_BIT * sizeof(size_t);

    mmap(NULL, BUFFER_SIZE, PROT_WRITE, PROT_WRITE, shared_fd, 0);

    sem_t* sem_shared = new sem_t;

    sem_t* sem_full = sem_open("/full", O_CREAT, 777, BUFFER_SIZE);
    sem_t* sem_empty = sem_open("/empty", O_CREAT, 777, 0);
    sem_init(sem_shared, 0, 1);

    while (true)
    {
        sem_wait(sem_empty);
        sem_wait(sem_shared);
        
        shared_pointer[*shared_fullness - 1] = '/0';
        --*shared_fullness;

        sem_post(sem_shared);
        sem_post(sem_full);
    }

    sem_destroy(sem_shared);
    sem_close(sem_empty);
    sem_close(sem_full);
    delete sem_shared;
    sem_unlink("/empty");
    sem_unlink("/full");
    shm_unlink("/prod-cons-buffer");
}
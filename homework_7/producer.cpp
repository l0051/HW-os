#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>

const size_t BUFFER_SIZE = 100; // page size !

int main()
{
    int shared_fd = shm_open("/prod-cons-buffer", O_CREAT | O_WRONLY, S_IWGRP);

    sem_t* sem_shared = new sem_t;

    sem_t* sem_full = sem_open("/full", O_CREAT, 777, BUFFER_SIZE);
    sem_t* sem_empty = sem_open("/empty", O_CREAT, 777, 0);
    sem_init(sem_shared, 0, 1);

    while (true)
    {
        sem_wait(sem_full);
        sem_wait(sem_shared);
        char* buffer = new char[BUFFER_SIZE];
        write(shared_fd, buffer, BUFFER_SIZE);
        sem_post(sem_shared);
        sem_post(sem_empty);
    }
    
    sem_destroy(sem_shared);
    sem_close(sem_empty);
    sem_close(sem_full);
    
    sem_unlink("/empty");
    sem_unlink("/full");
    //int shm_unlink(const char *name); //?
}
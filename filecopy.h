#ifndef FILECOPY_H
#define FILECOPY

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define RING_SIZE 5

void* producer(void *args);
void* consumer(void *args);

#endif

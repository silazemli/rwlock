#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "rwlock.h"

#define NUM_READERS 50
#define NUM_WRITERS 10
#define ITERATIONS 5000

rwlock_t lock;
volatile int shared_resource = 0;
volatile int lock_fails_on_read = 0;
volatile int lock_fails_on_write = 0;

void* reader(void* arg) {
    int id = *(int*)arg;
    int value;
    for (int i = 0; i < ITERATIONS; i++) {
        rwlock_begin_read(&lock);

        value = shared_resource;

        usleep(rand() % 100);
        
        if (value != shared_resource) lock_fails_on_read++;
        
        rwlock_end_read(&lock);

        usleep(rand() % 100);
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    int value;
    for (int i = 0; i < ITERATIONS; i++) {
        rwlock_begin_write(&lock);
        
        value = shared_resource;
        shared_resource++;
        
        usleep(rand() % 100);
        
        value++;
        if (value != shared_resource) lock_fails_on_write++;
        
        rwlock_end_write(&lock);
        
        usleep(rand() % 100);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];

    rwlock_init(&lock);

    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    printf("Final shared resource value: %d\n", shared_resource);
    printf("Lock fails on read: %d\n", lock_fails_on_read);
    printf("Lock fails on write: %d\n", lock_fails_on_write);

    return 0;
}

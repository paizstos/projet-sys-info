#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

int count_read = 0;
int count_write = 0;
int nthreads;

int nReaders;
int nWriters;

sem_t readsem;
sem_t writesem;

pthread_mutex_t lock;
pthread_mutex_t mutex_readcount;
pthread_mutex_t mutex_writecount;


void *writer() {
    for (int i = 0; i < 640; i++) {
        pthread_mutex_lock(&mutex_writecount);
        count_write++;
        if (count_write == 1) {
            sem_wait(&readsem);}
        pthread_mutex_unlock(&mutex_writecount);
        sem_wait(&writesem);
        for (int i = 0; i<10000; i++); // simulate writing
        sem_post(&writesem);
        pthread_mutex_lock(&mutex_writecount);
        count_write--;
        if (count_write == 0) {
            sem_post(&readsem);}
        pthread_mutex_unlock(&mutex_writecount);
    }
    return EXIT_SUCCESS;
}

void *reader() {
    for (int i = 0; i < 2560; i++) {
        pthread_mutex_lock(&lock);
        sem_wait(&readsem);
        pthread_mutex_lock(&mutex_readcount);
        count_read++;
        if (count_read == 1) {
            sem_wait(&writesem);}
        pthread_mutex_unlock(&mutex_readcount);
        sem_post(&readsem);
        pthread_mutex_unlock(&lock);
        for (int i = 0; i<10000; i++); // simulate reading
        pthread_mutex_lock(&mutex_readcount);
        count_read--;
        if (count_read == 0){
            sem_post(&writesem);}
        pthread_mutex_unlock(&mutex_readcount);
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Two arguments are required.\n");
        return EXIT_FAILURE;
    }

    nReaders = (int) strtol(argv[1], (char **) NULL, 10);
    nWriters = (int) strtol(argv[2], (char **) NULL, 10);

    if (nReaders < 1 || nWriters < 1) {
        fprintf(stderr, "The number of readers/writers must be at least 1.\n");
        return EXIT_FAILURE;
    }

    int err;

    err = sem_init(&readsem, 0, 1);
    if (err!=0){
        fprintf(stderr,"sem_init readers");
        return EXIT_FAILURE;
    }
    err = sem_init(&writesem, 0, 1);
        if (err!=0){
        fprintf(stderr,"sem_init writers");
        return EXIT_FAILURE;
    }

    pthread_t *readers;
    pthread_t *writers;

    readers = (pthread_t *) malloc(sizeof(pthread_t) * nReaders);
    writers = (pthread_t *) malloc(sizeof(pthread_t) * nWriters);

    for (int i = 0; i < nWriters; i++) {
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    for (int i = 0; i < nReaders; i++) {
        pthread_create(&readers[i], NULL, reader, NULL);
    }

    for (int i = 0; i < nWriters; i++) {
        pthread_join(writers[i], NULL);
    }

    for (int i = 0; i < nReaders; i++) {
        pthread_join(readers[i], NULL);
    }

    sem_destroy(&readsem);
    if (err != 0){
        fprintf(stderr,"sem_destroy_empty");
    }

    sem_destroy(&writesem);
    if (err != 0){
        fprintf(stderr,"sem_destroy_full");
    }

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&mutex_readcount);
    pthread_mutex_destroy(&mutex_writecount);
    sem_destroy(&readsem);
    sem_destroy(&writesem);

    free(readers);
    free(writers);

    return EXIT_SUCCESS;
}
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "pthread.h"
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>

int buf [8];

int count = 0;
int count_prod = 0;
int count_cons = 0;

int nProducer;
int nConsumer;

pthread_mutex_t mutex;
pthread_mutex_t consuming;
pthread_mutex_t producing;
sem_t empty;
sem_t full;



void *producer(){
    int item;
    while (true){
        if (item >= 8192){ 
            return NULL;}
        pthread_mutex_lock(&producing);
        item = count_prod++;
        pthread_mutex_unlock(&producing);
        for (int i=0; i<10000; i++); //simulating producing
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buf[count] = rand();
        count ++;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}


void *consumer(){
    int item;
    while (true){
        if (item >= 8192){ 
            return NULL;}
        pthread_mutex_lock(&consuming);
        item = count_cons++;
        pthread_mutex_unlock(&consuming);
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        count --;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        for (int i=0; i<10000; i++); //simulating consuming
    }
}


int main(int argc, char *argv[]){
    if (argc != 3) {
        fprintf(stderr, "Two arguments are required.\n");
        return EXIT_FAILURE;
    }

    nProducer = (int) strtol(argv[1], (char **) NULL, 10);
    nConsumer = (int) strtol(argv[2], (char **) NULL, 10);

    if (nProducer < 1 || nConsumer < 1) {
        fprintf(stderr, "The number of producer/consumer must be at least 1.\n");
        return EXIT_FAILURE;
    }

    pthread_t *thread_prod;
    pthread_t *thread_cons;

	thread_prod = (pthread_t *) malloc(sizeof(pthread_t) * nProducer);
	thread_cons = (pthread_t *) malloc(sizeof(pthread_t) * nConsumer);
	
    int err;
    
    err = sem_init(&empty, 0, 8);
    if (err != 0){
        fprintf(stderr,"sem_init empty");
    }

    err = sem_init(&full, 0, 0);
    if (err != 0){
        fprintf(stderr,"sem_init full");
    }  

    for (int i = 0; i < nProducer; i++){
        err = pthread_create(&(thread_prod[i]), NULL, producer, NULL);
        if (err != 0){
        fprintf(stderr,"Code 2");
        }
    }

    for (int i = 0; i < nConsumer; i++){
        err = pthread_create(&(thread_cons[i]), NULL, consumer, NULL);
        if (err != 0){
        fprintf(stderr,"Code 2");
        }
    }

    for (int i = 0; i < nProducer; i++){
        err = pthread_join((thread_prod[i]), NULL);
        if (err != 0){
        fprintf(stderr,"Code join prod");
        }
    }

    for (int i = 0; i < nConsumer; i++){
        err = pthread_join((thread_cons[i]), NULL);
        if (err != 0){
        fprintf(stderr,"Code join cons");
        }
    }

    sem_destroy(&empty);
    if (err != 0){
        fprintf(stderr,"sem_destroy_empty");
    }

    sem_destroy(&full);
    if (err != 0){
        fprintf(stderr,"sem_destroy_full");
    }

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&consuming);
    pthread_mutex_destroy(&producing);

    free(thread_cons);
    free(thread_prod);


    return(EXIT_SUCCESS);
}
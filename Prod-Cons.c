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
sem_t empty;
sem_t full;



void *producer(){

    int item;
    while (true){
        if (count_prod == 8192){ return NULL;}
        item = rand();
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buf[count] = item;
        count ++;
        count_prod ++;
        //printf("On a produit %d objets /n  ", count_prod);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        for (int i=0; i<10000; i++);
    }

}


void *consumer(){

    int item;
    while (true){
        if (count_cons == 8192){ return NULL;}
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        item = buf[count-1];
        count --;
        count_cons ++;
        //printf("consumed %d /n  ", count_cons);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        for (int i=0; i<10000; i++);
    }
}


int main(int argc, char *argv[]){

    pthread_mutex_init (&mutex, NULL);

    nProducer = (int) strtol(argv[1], (char **) NULL, 10);
    nConsumer = (int) strtol(argv[2], (char **) NULL, 10);

	pthread_t thread_prod [nProducer];
	pthread_t thread_cons [nConsumer];
	
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

    return(EXIT_SUCCESS);

}
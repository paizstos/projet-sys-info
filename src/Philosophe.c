
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int nPhilo;
pthread_t * phil;
pthread_mutex_t  *baguette;

void manger(int id){
    //printf("Philosophe [%d] mange\n", id);
}

void *philosophe(void* arg){
    int id = *(int*) arg;
    free(arg);
    int left = id;
    int right = (left + 1) % nPhilo;
    for (int i=0; i<100000; i++){
        if(left < right){
            pthread_mutex_lock(&baguette[left]);
            pthread_mutex_lock(&baguette[right]);
        }
        else{
            pthread_mutex_lock(&baguette[right]);
            pthread_mutex_lock(&baguette[left]);

        }
        manger(id);
        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);
    }
    return (NULL);
}

int main(int argc,char * argv[]){
    if (argc >= 2){
        nPhilo = (int) strtol(argv[1], (char **)NULL, 10);
    }
    else{
        return 1;
    }
    int err;
    pthread_t arr[nPhilo];
    pthread_mutex_t tab[nPhilo];
    phil = arr;
    baguette = tab;

    for(int i=0; i<nPhilo; i++){
        err = pthread_mutex_init(&(baguette[i]), NULL);
        if(err != 0){
            return 2;
        }
    }
    
    for(int i=0; i<nPhilo; i++){
        int * id = (int *) malloc(sizeof(int));
        *id = i;
	    err = pthread_create(&(phil[i]), NULL, &philosophe, id);
        if (err!=0){
            return 3;
        }
    }
    for(int i=nPhilo-1; i>=0; i--){
        err = pthread_join(phil[i], NULL);
        if (err!=0){
            return 4;
        }
    }
    err = pthread_mutex_destroy(baguette);
    if(err!=0){
        return 5;
    }
    return(EXIT_SUCCESS);
}

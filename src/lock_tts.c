#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "../lock/lock.c"

int N;
int l;

void *process(){
    for (int i = 0; i < 6400/N; i++){
        lock_tts(&l);
        for (int j = 0; j < 100000; j++); //simulate processing
        unlock(&l);
    }
    return EXIT_SUCCESS;
}

int main(int argc, char const *argv[])
{
    pthread_t *threads;

    if (argc != 2){
        fprintf(stderr, "One argument is required.\n");
        return EXIT_FAILURE;
    }

    N = (int) strtol(argv[1], (char **)NULL, 10);

    if (N <=0){
        fprintf(stderr, "Number of threads needs to be >= 1.\n");
        return EXIT_FAILURE;
    }

    init(&l);
    threads = (pthread_t *) malloc(sizeof(pthread_t) *N);

    for (int i = 0; i<N;i++){
        pthread_create(&threads[i], NULL, process, NULL);
    }

    for (int i = 0; i<N;i++){
        pthread_join(threads[i], NULL);
    }

    free(threads);
}

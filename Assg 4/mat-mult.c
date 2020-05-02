#include "mat-mult.h"
#include <stdio.h> //just to print stuff in screen
#include <time.h>
#include <pthread.h>
#define BILLION  1000000000.0

pthread_mutex_t mutex;//mutex lock
int s;
int *a, *b, *c;
int i=-1;//shared counter used by all threads

// This is the main function that represents each thread
void *thread()
{
    //Threads should access this area one at a time so mutex lock used
    pthread_mutex_lock(&mutex);
    int x= ++i;
    pthread_mutex_unlock(&mutex);

    while(x<s){
        for(int j=0;j<s;j++){
            for(int k=0;k<s;k++){
                //calculate the xth row
                *(c+x*s+j) += *(a+x*s+k) * *(b+k*s+j);
            }
        }
        //Increment shared counter so that next row can be calculated
        pthread_mutex_lock(&mutex);
        x= ++i;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);

    // execute till global i reaches row size.
    // Thus, the thread that finishes executing first will get the next i
    // and increase it so that next thread gets up to date i.


    pthread_exit(0);
}

void multiply(int* mat_ptr, int threads, int size){

    struct timespec start, end;

    a=mat_ptr;
    s=size;
    b=(a+size*size);
    c=(b+size*size);

    // note start time
    clock_gettime(CLOCK_REALTIME, &start);
    // driver thread that spawns threads and joins them after their completion

    pthread_mutex_init(&mutex, NULL);
    pthread_t tid[threads];

    // spawn threads
    for(long i=0; i<threads; i++)
    {
        int ret=pthread_create(&tid[i],NULL, thread, NULL);
        if(ret){
            printf("Error during thread creation.\n");
            exit(-1);
        }
    }

    // calling join will pause the main thread till the execution of the argument thread is not complete.
    for(long i=0;i<threads;i++)
    {
        pthread_join(tid[i],NULL);
    }
    // note end time
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (end.tv_sec - start.tv_sec) +
                            (end.tv_nsec - start.tv_nsec) / BILLION;

    //printf("Computation Time = %0.4f\n", time_spent);
}

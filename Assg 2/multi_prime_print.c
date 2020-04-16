#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

const long UPPER = 10000000;

typedef struct
{
    pthread_mutex_t *pmtx ;
    int *count ;
} Args;

int isPrime(long number)
{
    long limit = sqrt(number), i;

    for(i=2; i<=limit; i++)
    {
        if(number%i == 0) return 0;
    }

    return 1;
}

void* primePrint(void *args)
{
    Args *a = (Args*)args;
    long number;

    while(1)
    {
        pthread_mutex_lock(a->pmtx);
        number = *(a->count);
        *(a->count)=*(a->count)+1;
        pthread_mutex_unlock(a->pmtx);

        if(number > UPPER)
            break;

        if(isPrime(number) == 1);
            // printf("%ld : %d\n", number, pthread_self());
            
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int numThreads = atoi(argv[1]);
    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    FILE *fp;
    fp = fopen("output.txt", "a+");

    pthread_t threads[numThreads] ;
    pthread_mutex_t mutex ;
    pthread_mutex_init(&mutex, NULL);

    int shared_count=0, ret, i;
    Args arg_thread = {&mutex, &shared_count};
    

    for(i=0; i<numThreads; ++i)
    {
        ret=pthread_create(&threads[i], NULL, &primePrint, (void*)&arg_thread);

        if(ret)
        {
            printf("Error creating thread\n");
            exit(-1);
        }
    }

    for(i=0; i<numThreads; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0; 
    
    fprintf(fp, "%d : %lf\n", numThreads, elapsed);
    fclose(fp);
    printf("%lf\n", elapsed);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

const long UPPER = 10000000;

typedef struct
{
    int *availableThreadID, *current;
    pthread_mutex_t *pmtx;
    Arg(int *a, int *c, pthread_mutex_t *p)
    {
        availableThreadID = a;
        current = c;
        pmtx = p;
    }
}Arg;

void* isPrime(void* arg)
{
    Arg *args = (Arg*)arg;
    int number = args->current;
    long limit = sqrt(number), i;

    for(i=2; i<=limit; i++)
    {
        if(number%i == 0) return 0;
    }

    return 1;
}

void primePrint(int numThreads)
{
    long i = 2;
    int availableThreadID = 0;
    FILE *fp;
    fp = fopen("output.txt", "w+");
    pthread_t threads[numThreads];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    Arg()
    while(true)
    {
        
    }


    fclose(fp);
}

int main(int argc, char *argv[])
{
    clock_t start, end;
    
    int numThreads = (int)argv[1];

    start = clock();
    primePrint(numThreads);
    end = clock();

    printf("%lf\n", (double)((end - start)/CLOCKS_PER_SEC));

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>


typedef struct Args
{
    pthread_mutex_t *pmtx;
    int *ctr, N, *a;
}Args;

void* transposeSingle(void* a)
{
    Args *arguments = (Args*)a;
    int ctr, i, j, n, temp, *matptr;

    matptr = arguments->a;

    while(1)
    {
        pthread_mutex_lock(arguments->pmtx);
        ctr = *arguments->ctr;
        *arguments->ctr = ctr+1;
        pthread_mutex_unlock(arguments->pmtx);

        n = arguments->N;

        i = floor((double)ctr/(double)n);
        j = ctr%n;
        // printf("%d %d %x\n", i, j, pthread_self());

        if(ctr>n*n - 1) break;
        if(j <= i) continue;

        temp = *(matptr + i*n + j);
        *(matptr + i*n + j) = *(matptr + j*n + i);
        *(matptr + j*n + i) = temp;
    }

    pthread_exit(NULL);
}

void fillMatrix(int *mat, int n)
{
    long i;

    for(i=0; i<n*n; i++)
    {
        *(mat + i) = i;
    }
}

// prints the matrix
void printMatrix(int *mat, int n)
{
    int i, j;

    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            printf("%d \n", *(mat + i*n + j));
        }
        //printf("\n");
    }
}

int main(int argc, char *argv[])
{
    FILE *fp;
    // clock variables to calculate execution time of a program
    struct timespec start, finish;
    double elapsed;
    int N = atoi(argv[1]);
    int numthreads = atoi(argv[2]);
    int *matPtr = (int*)malloc(N*N*sizeof(int));
    pthread_t threads[numthreads];
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    int i;

    int ctr = 0;
    Args arguments = {&mutex, &ctr, N, matPtr};

    fillMatrix(matPtr, N);
    // printMatrix(matPtr, N);
    // printf("\n");
    //printf("before\n");
    // use of clock functionalities in c to obtain start time of execution
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(i=0; i<numthreads; i++)
    {
        pthread_create(&threads[i], NULL, &transposeSingle, (void*)&arguments);
    }

    for(i=0; i<numthreads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    //printf("after\n");
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    if(N==8)
        printMatrix(matPtr, N);
    fp = fopen("fine_output.txt", "a+");

    // prints the execution time to file
    fprintf(fp, "%lf\n", elapsed);
    //printf("%lf\n", elapsed);
    fclose(fp);
    free(matPtr);

    return 0;
}

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef struct Args
{
    pthread_mutex_t *pmtx ;
    int *a, *currentRow, m, n;
}Args;

void myPrint(int *a, int N)
{
    int i, j;

    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            printf("%d ", *(a + i*N + j));
        }
        printf("\n");
    }
}

void multiply (int *a, int i, int m, int n)
{

    //getting location of start memories of matrices stored in memory
    int *matA = a, *matB = (matA + m*n), *matC = (matB + m*n);
    int j, k;

    // printf("row is: %d\n", i);

    for(j=0; j<n; j++)
    {
        for(k = 0; k<m; k++)
        {
            // printf("%d*%d\n", *(matA + i*n + k),(*(matB + k*n + j)));
            *(matC + i*n + j) += *(matA + i*n + k)*(*(matB + k*n + j));
        }
    }

    return;
}

void *call_parallel(void *args){
    int *a, rowCounter, m, n;
    Args *arguments = (Args*)args;
    a = arguments->a;
    m = arguments->m;
    n = arguments->n;

    while(1)
    {
        pthread_mutex_lock(arguments->pmtx);
        rowCounter = *arguments->currentRow;
        *(arguments->currentRow) = rowCounter+1;
        pthread_mutex_unlock(arguments->pmtx);
        // printf("%d\n", rowCounter);
        if(rowCounter>=m) break;
        multiply(a, rowCounter, m, n);
    }

    pthread_exit(NULL);
}
int A[1024*3][1024];
int main(int argc, char *argv[])
{
    int N = atoi(argv[1]);
    int numThreads = atoi(argv[2]);
    int i=0, j, k;
    time_t t;
    struct timespec start, finish;
    double elapsed;
    
    // int *matA = (int*)malloc(3*N*N);
    int *matA = &A;
    memset((void*)(matA + 2*N*N), 0, N*N);
    int *matB = matA + N*N;
    int *matC = matB + N*N;
    pthread_t threads[numThreads];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    
    srand((unsigned) time(&t));

    for(k=0; k<2; k++)
    {
        for(i=0; i<N; i++)
        {
            for(j=0; j<N; j++)
            {
                *((matA + i*N) + j + N*N*k) = rand() % 10;
            }
        }
    }
    
    // myPrint(matA, N);
    // printf("\n");
    // myPrint(matB, N);
    // printf("\n");

    int rowCounter = 0;
    Args arguments = {&mutex, matA, &rowCounter, N, N};

    clock_gettime(CLOCK_MONOTONIC, &start);
    for(i=0; i<numThreads; i++)
    {
        pthread_create(&threads[i], NULL, &call_parallel, (void*)&arguments);
    }
    


    for(i=0; i<numThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("here\n");
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0; 

    // myPrint(matC, N);

    printf("\n%lf\n", elapsed);
    // free(matA);
    return 0;
}
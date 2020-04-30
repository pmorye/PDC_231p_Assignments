#include "mat-mult.h"
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// struct to bind mutext lock with the row counter and to pass other variables
typedef struct Args
{
    pthread_mutex_t *pmtx ;
    int *a, *currentRow, m, n;
}Args;

// thread function to calculate 1 row of mat C
void* row_multiply(void* args)
{
    int *a, rowCounter, m, n;
    Args *arguments = (Args*)args;
    a = arguments->a;
    m = arguments->m;
    n = arguments->n;

    //getting location of start memories of matrices stored in memory
    int *matA = a, *matB = (matA + m*n), *matC = (matB + m*n);

    while(1)
    {
        // lock the row counter and update it after getting its value
        pthread_mutex_lock(arguments->pmtx);
        rowCounter = *arguments->currentRow;
        *(arguments->currentRow) = rowCounter+1;
        pthread_mutex_unlock(arguments->pmtx);

        // break if all rows have been calculated
        if(rowCounter>=m) break;
        int j, k;

        for(j=0; j<n; j++)
        {
            for(k = 0; k<m; k++)
            {
                // pointer arithmetic to get rows and columns
                *(matC + rowCounter*n + j) += *(matA + rowCounter*n + k)*(*(matB + k*n + j));
            }
        }
    }

    pthread_exit(NULL);
}

void multiply(int* mat_ptr, int threads, int size)
{
    int i=0, j, k;
    time_t t;
    struct timespec start, finish;
    double elapsed;

    pthread_t threadsarray[threads];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    
    srand((unsigned) time(&t));

    int rowCounter = 0;
    Args arguments = {&mutex, mat_ptr, &rowCounter, size, size};

    //start measuring time
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(i=0; i<threads; i++)
    {
        pthread_create(&threadsarray[i], NULL, &row_multiply, (void*)&arguments);
    }
    
    for(i=0; i<threads; i++)
    {
        pthread_join(threadsarray[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    //stop measuring time

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0; 

    printf("\n%lf\n", elapsed);

    printf("threads: %d, matrix: %dx%d\n", threads, size, size);
    
    //write outputs to file
    FILE *fp;
    fp = fopen("output.txt", "a+");
    fprintf(fp, "%lf\n", elapsed);
    fclose(fp);
    
    return;
}
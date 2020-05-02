#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>


// initializes the matrix with values
void fillMatrix(int *mat, int n)
{
    long i;

    for(i=0; i<n*n; i++)
    {
        *(mat + i) = i;
    }
}

//calculates the transpose of the matrix
void transposeMatrix(int *mat, int n)
{
    int i, j, temp;

    for(i=0; i<n; i++)
    {
        for(j=i+1; j<n; j++)
        {
            temp = *(mat + i*n + j);
            *(mat + i*n + j) = *(mat + j*n + i);
            *(mat + j*n + i) = temp;
        }
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
            printf("%d ", *(mat + i*n + j));
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    FILE *fp;
    // clock variables to calculate execution time of a program
    struct timespec start, finish;
    double elapsed;
    int N = atoi(argv[1]);
    int *matPtr = (int*)malloc(N*N*sizeof(int));
    
    fillMatrix(matPtr, N);
    // printMatrix(matPtr, N);
    // printf("\n");
    
    // use of clock functionalities in c to obtain start time of execution
    clock_gettime(CLOCK_MONOTONIC, &start);
    transposeMatrix(matPtr, N);
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0; 

    // printMatrix(matPtr, N);
    fp = fopen("single_output.txt", "a+");

    // prints the execution time to file
    fprintf(fp, "%lf\n", elapsed);
    fclose(fp);
    free(matPtr);

    return 0;
}
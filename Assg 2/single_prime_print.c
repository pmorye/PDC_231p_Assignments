#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

FILE *fp;

// function to check if the number is a prime number
int isPrime(long number)
{
    long limit = sqrt(number), i;
    // to reduce computations, our for loop runs till the square root of the number
    for(i=2; i<=limit; i++)
    {
        if(number%i == 0) return 0;
    }

    return 1;
}

// function to print all prime numbers upto UPPER limit
void primePrint()
{
    long i;
    fp = fopen("single_output.txt", "w+");
    // checks if each number incremented by a factor of 1 is a prime
    for(i=2; i<=UPPER; i++)
    {
        // print the output in file single_output.txt
        if(isPrime(i) == 1)
        {
            fprintf(fp, "%ld\n", i);
        }
    }
}

int main()
{
    // clock variables to calculate execution time of a program
    clock_t start, end;
    
    start = clock();
    primePrint();
    end = clock();

    // prints the execution time
    fprintf(fp, "%lf\n", (double)((end - start)/CLOCKS_PER_SEC));
    fclose(fp);

    return 0;
}
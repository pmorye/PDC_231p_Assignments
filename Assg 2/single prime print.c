#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

const long UPPER = 10000000;

int isPrime(long number)
{
    long limit = sqrt(number), i;

    for(i=2; i<=limit; i++)
    {
        if(number%i == 0) return 0;
    }

    return 1;
}

void primePrint()
{
    long i;
    FILE *fp;
    fp = fopen("output.txt", "w+");

    for(i=3; i<=UPPER; i++)
    {
        if(isPrime(i) == 1)
            fprintf(fp, "%ld\n", i);
    }

    fclose(fp);
}

int main()
{
    clock_t start, end;
    
    start = clock();
    primePrint();
    end = clock();

    printf("%lf\n", (double)((end - start)/CLOCKS_PER_SEC));

    return 0;
}
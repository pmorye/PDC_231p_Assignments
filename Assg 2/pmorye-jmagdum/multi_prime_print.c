#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

// setting the upper bound
const long UPPER = 10000000;
FILE *fp;

// defining a struct to maintain a shared count between threads along with mutex
typedef struct
{
    pthread_mutex_t *pmtx ;
    int *count ;
} Args;

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

// function to print all prime numbers from multiple threads
void* primePrint(void *args)
{
    Args *a = (Args*)args;
    long number;

    // while loop runs till number reaches the upper bound
    while(1)
    {
    	// lock indicates that at a given time, only one thread will have access to the shared count
        pthread_mutex_lock(a->pmtx);

        //get number which we have to check for primality
        number = *(a->count);

        // shared count is increased by 1
        *(a->count)=*(a->count)+1;

        // unlocks the mutex indicating that it is not owned by any thread
        pthread_mutex_unlock(a->pmtx);

        if(number > UPPER)
            break;

        //print number to file multi_output.txt
        if(isPrime(number) == 1)
        {
            fprintf(fp, "%ld\n", number);
        }
            
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	// accept the number of threads as a command line argument
    int numThreads = atoi(argv[1]);
    struct timespec start, finish;
    double elapsed;

    // use of clock functionalities in c to obtain start time of execution
    clock_gettime(CLOCK_MONOTONIC, &start);

    // File pointer initializations for output to be written to a file
    fp = fopen("multi_output.txt", "a+");

    // create an array of pthreads
    pthread_t threads[numThreads];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    int shared_count = 2, ret, i;
    // create an object of Args struct
    Args arg_thread = {&mutex, &shared_count};
    
    // create the threads taken as a command line argument and store their ids
    for(i=0; i<numThreads; ++i)
    {
        ret=pthread_create(&threads[i], NULL, &primePrint, (void*)&arg_thread);

        if(ret)
        {
            printf("Error creating thread\n");
            exit(-1);
        }
    }

    // once the threads have executed their function specified during creation, we use join to wait for their termination
    for(i=0; i<numThreads; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    // use of clock functionalities in c to obtain finish time of execution
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0; 
    
    // write output to the file
    fprintf(fp, "%lf\n", elapsed);
    fclose(fp);
    printf("%lf\n", elapsed);

    return 0;
}
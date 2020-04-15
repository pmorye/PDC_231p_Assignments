#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

double EPSILON = 0.0002;


//this function fills the request array with random module numbers uniformly
void uniform_distribution(int request[], int p, int m ) {
   
    int i;
    // printf("%d\n", m);

   for ( i = 0; i < p; ++i) {
        if(request[i] == -1)\
        {
            request[i] = rand()%m;
            // printf( "r[%d] = %d\n", i, request[i]);
        }
   }
}

//helper function for normal distribution
double rand_normal(double mean, double stddev)
{//Box muller method
    static double n2 = 0.0;
    static int n2_cached = 0;
    if (!n2_cached)
    {
        double x, y, r;
        do
        {
            x = 2.0*rand()/RAND_MAX - 1;
            y = 2.0*rand()/RAND_MAX - 1;

            r = x*x + y*y;
        }
        while (r == 0.0 || r > 1.0);
        {
            double d = sqrt(-2.0*log(r)/r);
            double n1 = x*d;
            n2 = y*d;
            double result = n1*stddev + mean;
            n2_cached = 1;
            return result;
        }
    }
    else
    {
        n2_cached = 0;
        return n2*stddev + mean;
    }
}

//this function takes the mean array and generates normally distributed module numbers around these mean values
void normal_distribution(int request[],int mean[], int p, int m){

  int stddev = m/6, i;
  for(i=0;i<p;i++){
    if(request[i] == -1){
      unsigned int val = round(rand_normal((double)mean[i], (double)stddev));
      request[i] = val%m;
    //   printf( "r[%d] = %d\n", i, request[i]);
    }
  }
}

//main driver function
int main(int argc, char *argv[])
{
    time_t t;
    /* set the seed */
    srand((unsigned)time( &t ));
    char d;
    int p, convergingCycleCnt = 0, idxOfFirstFail = -1;
    int proc, cycle, i, j;

    p = atoi(argv[1]);
    d = *(char*)argv[2];

    int requests[p], mapping[p];

    FILE *fp_u, *fp_n;
    
    if(d == 'u')
    {
        fp_u = fopen("o-p_u.txt", "w+");
    }
    else
    {
        fp_n = fopen("o-p_n.txt", "w+");
    }
    

    //loop through the module numbers
    for(i=1; i<=2048; i++)
    {
        for(j=0; j<p; j++)
        {
            requests[j] = -1;
        }
        if(d == 'n')
        uniform_distribution(mapping, p, i);
        double lastAvg = 0, current;
        long requestGrantCtr[p];
        int ctrpos;
        for(ctrpos = 0; ctrpos<p; ctrpos++)
        {
            requestGrantCtr[ctrpos] = 0;
        }

        //for each simulation, calculate and write the averages
        for(cycle = 1; cycle <= 1000000; cycle++)
        {
            int modules[2049] = {0};
        
            if(convergingCycleCnt > 10)          //if terminating condition is truethen break
            {
                convergingCycleCnt = 0;
                break;
            }

            if(d == 'u')
                uniform_distribution(requests, p, i);
            if(d == 'n')
                normal_distribution(requests, mapping, p, i);


            //loop through processor and requests and keep track of first unsuccessful request
            int pos = idxOfFirstFail == -1 ? 0 : idxOfFirstFail;
            idxOfFirstFail = -1;
            double avgSum = 0;
            int toSkip = 0;

            for(proc = 0; proc < p; proc++)
            {
                if(modules[requests[pos]] == 0)
                {
                    modules[requests[pos]] = 1;
                    // printf("%d\n", pos);
                    requests[pos] = -1;
                    requestGrantCtr[pos]++;
                }
                else
                {
                    idxOfFirstFail = idxOfFirstFail == -1 ? pos : idxOfFirstFail;
                }

                if(requestGrantCtr[pos] == 0)
                    toSkip = 1;
                else
                {
                    avgSum += (double)cycle / (double)requestGrantCtr[pos];
                    // printf("%d %ld\n", cycle, requestGrantCtr[pos]);
                    // printf("%f\n", avgSum);
                }
                pos = (pos + 1) % p;
            }

            // if there is even 1 processor which hasn't yet been granted, then skip calculation for this cycle
            if(toSkip) {
                toSkip = 0;
                continue;
            }

            current = avgSum / (double)p;

            if(lastAvg == 0) {
                lastAvg = current;
                continue;
            }

            //calculate the terminating condition and compare it to Epsilon value
            if(fabs(1.0 - lastAvg / current) < EPSILON) 
            {
                lastAvg = current;
                convergingCycleCnt++;
            }
            else convergingCycleCnt = 0;
            lastAvg = current;

        }

        // printf("final %lf and cycles used are %d\n", lastAvg, cycle);
        if(d == 'u'){
            fprintf(fp_u, "%lf\n", lastAvg);
        }
        else
        {
            fprintf(fp_n, "%lf\n", lastAvg);
        }
        

    }

    if(d == 'u')
    {
        fclose(fp_u);
    }
    else
    {
        fclose(fp_n);
    }
    
    return 0;
}
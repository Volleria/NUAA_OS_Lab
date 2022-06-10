#include <stdio.h>
#include <pthread.h>

#define N 100000

double worker_outout = 0;
double master_output = 0;

void master()
{
    int i ,j;
    for ( i = 1; i <= N / 2 ; i++)
    {
        j = 2 * i -1;
        if( i % 2 == 0)
        {
            j = -j;
        }

        master_output += (double)1 / j;
    }
}
void *worker(void *arg)
{
    int i , j;
     for( i = N/2 + 1;i <=N ;i++ )
     {
         j = 2 * i - 1;
         if( i % 2== 0)
         {
             j = -j;
         }
          worker_outout += (double)1 / j;
     }
    return NULL;
}


int main()
{
    pthread_t worker_tid;

    double total , PI;
    pthread_create( &worker_tid, NULL,worker,NULL);
    master();
    pthread_join(worker_tid,NULL);

    PI = 4 * (worker_outout + master_output);

    printf("%lf\n" , PI);
    return 0;
}

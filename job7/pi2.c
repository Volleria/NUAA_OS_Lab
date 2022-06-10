#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int p_n = 10;

struct param
{
    int start;
    int end;
};

struct result
{
    double sum;
};

void *pi2(void * arg)
{
    struct param *param = (struct param *) arg;
    struct result *result = malloc(sizeof(struct result));
    int i ,j;
    double sum;

    for ( i = param->start; i <= param->end; i++)
    {
        j = 2 * i -1;
        if( i %2 == 0)
            j = -j;

        sum += (double)1 / j;
    }

    result->sum = sum;
    return result;    
}

int main()
{
    pthread_t workers[p_n];
    struct param params[p_n];
    int i;
    double PI = 0;

    for(i =0 ;i<p_n;i++)
    {
        struct param *param = &params[i];
        param->start = i * 100000 + 1;
        param->end = (i + 1) * 100000 ;
        pthread_create(&workers[i],NULL,pi2,param);
    }

    for(i =0 ; i <p_n; i++)
    {
        struct result *result;
        pthread_join(workers[i],(void **)&result);
        PI += result->sum;
        free(result);
    }

    printf("%f\n",4.0 * PI);

    return 0;


}
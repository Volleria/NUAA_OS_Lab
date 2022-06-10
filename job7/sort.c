#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int a[10]  = {5,6,1,3,7,4,9,8,2,10};
int b[10];

struct se
{
    int start;
    int end;
};

void * selection_sort(void *arg)
{

    int smallest;
    int temp;
    struct se * se = (struct se*)arg;
    for (int i = se->start; i < se->end; i++)
    {
        smallest = i;
        for(int j = i + 1;j < se->end;j++)
        {
            if (a[j] < a[smallest])
                smallest = j;
        }
        temp = a[i];
        a[i] = a[smallest];
        a[smallest] = temp;
    }

}

void merge()
{

    int i = 0;
    int j = 5;
    int k = 0;

    while (i < 5 && j < 10)
    {
        if(a[i] < a[j])
            b[k++] = a[i++];
        else
            b[k++] = a[j++];
    }
    while (i < 5)
    {
        b[k++]= a[i++];
    }
    while (j < 10 )
    {
        b[k++] = a[j++];
    }

}
int main()
{
    for (int i = 0; i < 10; i++)
    {
        printf("%d ",a[i]);
    }
    printf("\n");
    pthread_t workers[2];
    struct se ses[2];

    for(int i = 0;i<2;i++)
    {
        struct se *se = &ses[i];
        se->start = i * 5;
        se->end = (i+1) * 5;
        pthread_create(&workers[i],NULL,selection_sort,se);
    }

    pthread_join(workers[0],NULL);
    pthread_join(workers[1],NULL);
    merge();
    for (int i = 0; i < 10; i++)
    {
        printf("%d ",b[i]);
    }
    printf("\n");
    return 0;

}
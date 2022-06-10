#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define TIME 100
pthread_mutex_t  mutex;
pthread_cond_t wait_cond;
int flag = 1;
void *ping(void *arg)
{
    for(int i = 0 ; i < TIME ; i++)
    {
        pthread_mutex_lock(&mutex);
        while( flag != 1)
            pthread_cond_wait(&wait_cond,&mutex);        
        printf("ping\n");
        flag = 2;
        pthread_cond_signal(&wait_cond);
        pthread_mutex_unlock(&mutex);
    }
}
void *pong(void *arg)
{
    for(int i = 0 ; i < TIME ; i++)
    {
        pthread_mutex_lock(&mutex);
        while( flag != 2)
            pthread_cond_wait(&wait_cond,&mutex);       
        printf("pong\n");
        flag = 1;
        pthread_cond_signal(&wait_cond);
        pthread_mutex_unlock(&mutex);
    }
}
int main()
{
    pthread_t ping_tid,pong_tid;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&wait_cond,NULL);
    pthread_create(&ping_tid,NULL,ping,NULL);
    pthread_create(&pong_tid,NULL,pong,NULL);
    pthread_join(ping_tid,NULL);
    pthread_join(pong_tid,NULL);
    return 0;
}



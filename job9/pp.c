#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define TIME 100



typedef struct 
{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

void sema_init(sema_t *sema, int value)
{
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    ++sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex1;
sema_t mutex2;

void *ping(void *arg)
{
    for(int i = 0 ; i< TIME; i++)
    {
        sema_wait(&mutex1);
        printf("ping\n");
        sema_signal(&mutex2);
    }
}
void *pong(void * arg)
{
    for(int i = 0;i < TIME ;i++)
    {
        sema_signal(&mutex1);
        printf("pong\n");
        sema_wait(&mutex2); 
    }
}



int main()
{ 
    pthread_t ping_tid;
    pthread_t pong_tid;

    sema_init(&mutex1, 0);
    sema_init(&mutex2, 0);


    pthread_create(&ping_tid, NULL, ping, NULL);
    pthread_create(&pong_tid, NULL, pong, NULL);

    pthread_join(ping_tid, NULL);
    pthread_join(pong_tid, NULL);
    return 0;
}






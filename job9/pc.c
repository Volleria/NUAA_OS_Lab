#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define CAPACITY 4
int buffer1[CAPACITY];
int buffer2[CAPACITY];

int in1;
int out1;
int in2;
int out2;


int get_item1()
{
    int item;

    item = buffer1[out1];
    out1 = (out1 + 1) % CAPACITY;
    return item;
}

void put_item1(int item)
{
    buffer1[in1] = item;
    in1 = (in1 + 1) % CAPACITY;
}


int get_item2()
{
    int item;

    item = buffer2[out2];
    out2 = (out2 + 1) % CAPACITY;
    return item;
}

void put_item2(int item)
{
    buffer2[in2] = item;
    in2 = (in2 + 1) % CAPACITY;
}

typedef struct {
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

sema_t mutex_sema;
sema_t empty1;
sema_t full1;
sema_t empty2;
sema_t full2;

#define ITEM_COUNT (CAPACITY * 2)

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) 
    { 
        sema_wait(&full2);
        sema_wait(&mutex_sema);

        item = get_item2();
        printf("    consume: %c\n", item); 

        sema_signal(&mutex_sema);
        sema_signal(&empty2);
    }
}

void *produce()
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        sema_wait(&empty1);
        sema_wait(&mutex_sema);

        item = i + 'a';
        put_item1(item);
        printf("produce: %c\n", item); 

        sema_signal(&mutex_sema);
        sema_signal(&full1);
    }
}

void *compute(void * arg)
{
    for (int i = 0;i< ITEM_COUNT ; i++)
    {
        sema_wait(&full1);
        sema_wait(&empty2);
        sema_wait(&mutex_sema);

        int item = get_item1();
        printf("  compute: %c:",item);
        item = item - 32;
        printf("%c\n",item);
        put_item2(item);

        
        sema_signal(&mutex_sema);
        sema_signal(&full2);
        sema_signal(&empty1);
    }
}

int main()
{ 
    pthread_t consumer_tid;
    pthread_t computer_tid;

    sema_init(&mutex_sema, 1);

    sema_init(&empty1, CAPACITY );
    sema_init(&full1, 0);
    sema_init(&empty2, CAPACITY );
    sema_init(&full2, 0);

    pthread_create(&consumer_tid, NULL, consume, NULL);
    pthread_create(&computer_tid, NULL, compute, NULL);
    produce();
    pthread_join(consumer_tid, NULL);
    pthread_join(computer_tid, NULL);
    return 0;
}






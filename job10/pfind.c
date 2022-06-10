#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#define WORKER_NUMBER 4
#define CAPACITY 2


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

sema_t mutex;
sema_t full;
sema_t empty;

struct task
{
    int is_end;
    char path[128];
    char string[128];
}mytask[CAPACITY];

int head = 0;
int tail = 0;

void find_file(char *path, char *target)
{
    FILE *file = fopen(path, "r");

    char line[257];
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, target))
            printf("%s: %s", path, line);
    }
    fclose(file);
}


void *worker_entry(void *arg)
{
    while (1)
    {
        sema_wait(&full);
        sema_wait(&mutex);

        struct task task;
        task = mytask[head%CAPACITY];
        // printf("head:%d\n",head);
        head++;
        if ( task.is_end )
        {
            sema_signal(&mutex);
            sema_signal(&empty);
            break;         
        }

        find_file(task.path, task.string);

        sema_signal(&mutex);
        sema_signal(&empty);
    }
}

void find_dir(char *path,char *target)
{
    char newpath[257];
    DIR *dir = opendir(path);
    // printf("100\n");

    if(dir == NULL)
        return;
    struct dirent *entry;                      
    
    // printf("105\n");
    while (entry = readdir(dir))
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        sprintf(newpath , "%s/%s",path,entry->d_name);
        if (entry->d_type == DT_DIR)           
            find_dir(newpath,target);


        if (entry->d_type == DT_REG)
        {
            sema_wait(&empty);
            sema_wait(&mutex);
            // printf("newpath:%s\n",newpath);

            mytask[tail%CAPACITY].is_end = 0;
            strcpy(mytask[tail%CAPACITY].path,newpath);
            strcpy(mytask[tail%CAPACITY].string,target);
            // printf("tail:%d\n",tail);
            tail++;

            sema_signal(&mutex);
            sema_signal(&full);
        
        }           
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{
    char *path = argv[1];
    char *string = argv[2];

    struct stat info; 
    stat(path, &info);


    if (!S_ISDIR(info.st_mode))
    {
        find_file(path, string);
        return 0;
    }

    sema_init(&mutex,1);
    sema_init(&full,0);
    sema_init(&empty,CAPACITY);

    pthread_t works_tid[WORKER_NUMBER];
    for(int i = 0; i < WORKER_NUMBER; i++)
        pthread_create(&works_tid[i],NULL,worker_entry,NULL);  

    find_dir(path,string);

    for (int i = 0; i < WORKER_NUMBER ;i++)
    {
        sema_wait(&empty);
        sema_wait(&mutex);
        mytask[tail%CAPACITY].is_end = 1;
        tail++;
        sema_signal(&mutex);
        sema_signal(&full);
    }

    for(int i = 0; i < WORKER_NUMBER; i++)
        pthread_join(works_tid[i],NULL);  
    return 0;
}
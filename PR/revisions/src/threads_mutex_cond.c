#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N 4

pthread_mutex_t mutex [N];
pthread_cond_t  cond [N];
int flag[N];

void *thread_cond(void *args) {
    int id = (*(int *)args);
    free(args);
    
    if (id != 0)
    {
        pthread_mutex_lock(&mutex[id]);
        if (!flag[id])
        {
            pthread_cond_wait(&cond[id], &mutex[id]);
        }

        pthread_mutex_unlock(&mutex[id]);
    }
    printf ("Thread :%d\n",id);
    
    if (id!=N-1)
    {
        pthread_mutex_lock(&mutex[(id+1)]);
        
        flag[(id+1)]=1;
        pthread_cond_signal(&cond[(id+1)]);
        
        pthread_mutex_unlock(&mutex[(id+1)]);
    }
    return NULL;
}

int main(void)
{
    int i;
    pthread_t threads[N];
    int *targ;
    
    for(i=0; i<N; i++)
    {
        pthread_mutex_init(&mutex[i], NULL);
        pthread_cond_init(&cond[i],NULL);
        
        flag[i]=0;
        targ = malloc(sizeof(int));
        *targ = i;
        
        pthread_create(&threads[i], NULL, thread_cond, targ);
    }
    
    for(i=0; i<N; i++)
    {
       pthread_join(threads[i], NULL);
    }
    
    return EXIT_SUCCESS;
}
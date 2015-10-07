#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string.h>

#include <pthread.h>

#define NB_THREADS 10

int threadsWaiting;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void wait_barrier (int n) {
    
    pthread_mutex_lock (&m);
    threadsWaiting++;

    if(threadsWaiting < n) {
        pthread_cond_wait (&cond,&m);
    }
    else
        pthread_cond_broadcast(&cond);
    pthread_mutex_unlock (&m);
}


void* thread_func () {
   printf ("avant barriere\n");
   wait_barrier (NB_THREADS);
   printf ("après barriere\n");
   pthread_exit ( (void*)0);
}


int main(void)
{
    pthread_t tid[NB_THREADS];
    int i,status;
    
    threadsWaiting = 0;

    /* pthread_create */
    for(i=0;i<NB_THREADS;i++)
    {         
        if (pthread_create(&tid[i], NULL,thread_func, NULL) != 0) {
            perror("pthread_create \n");
            exit (1);
        }
    }
    
    
    /* pthread_join */
    for (i=0; i < NB_THREADS; i++) 
    {
        if (pthread_join(tid[i], (void**) &status) != 0) {
            perror("pthread_join"); 
            exit(1);
        }
        else
            printf ("Thread %d fini.\n",i);
    }
    


    
    return 0;
}

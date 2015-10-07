#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string.h>

#include <pthread.h>

void* thread_func (void *arg) {
   printf ("avant barriere\n");
   wait_barrier (NB_THREADS);
   printf ("après barriere\n");
   pthread_exit ( (void*)0);
}


int main(void)
{
    pthread_t tid[N+1];
    int i,status;
    
    flag = 0;
    global = 0;
        
    /* thread qui affiche la variable globale */
    if (pthread_create (&tid[N], NULL,display, NULL) != 0) {
        perror("pthread_create \n");
        exit (1);
    }
        
    /* pthread_create */
    for(i=0;i<N;i++)
    {         
        sleep(1);
        if (pthread_create(&tid[i], NULL,f, NULL) != 0) {
            perror("pthread_create \n");
            exit (1);
        }
    }
    
    /* pthread_join */
    for (i=0; i < N; i++) 
    {
        if (pthread_join(tid[i], (void**) &status) != 0) {
            perror("pthread_join"); 
            exit(1);
        }
        else
            printf ("Thread %d fini.\n",i);
    }

    flag = 1;
    
    pthread_mutex_lock (&m);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock (&m);  
    
    /* attendre dernier thread "display" */
    if (pthread_join(tid[N], (void**) &status) != 0) {
        printf ("pthread_join"); 
        exit (1);
    }
    else
        printf ("Thread %d fini.\n",i);
      
    
    return 0;
}

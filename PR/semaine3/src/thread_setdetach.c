#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string.h>

#include <pthread.h>

#define N 5

int global,cpt;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *display()
{
    pthread_mutex_lock (&m);
    while (0 < cpt) {
        pthread_cond_wait (&cond,&m);
    }
    pthread_mutex_unlock (&m);

    printf("global: %d\n",global);
    
    pthread_exit (NULL);
}


void *f()
{
    int r;
    
    srand(time(NULL));  
    r = (int) (10*((double)rand())/ RAND_MAX);  
        
    printf("alea: %d, tid: %d\n",r,(int)pthread_self());
    
    pthread_mutex_lock (&m);  
    global += r; 
    pthread_mutex_unlock (&m);
    
    pthread_mutex_lock (&m2);  
    cpt--; 
    pthread_mutex_unlock (&m2);
    
    if(cpt == 0)
    {
        pthread_mutex_lock (&m);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock (&m);  
    }
    
    pthread_exit (NULL);
}

int main(void)
{
    pthread_attr_t attr;
    pthread_t tid[N+1];
    int i,status;
    
    
    cpt = N;
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
        
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        
        if (pthread_create(&tid[i], &attr,f, NULL) != 0) {
            perror("pthread_create \n");
            exit (1);
        }
    }
        
    
    /* attendre dernier thread "display" */
    if (pthread_join(tid[N], (void**) &status) != 0) {
        printf ("pthread_join"); 
        exit (1);
    }
    else
        printf ("Thread %d fini.\n",i);
      
    
    return 0;
}

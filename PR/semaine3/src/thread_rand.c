#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string.h>

#include <pthread.h>

#define N 5

int global;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *f()
{
    int r;
    
    srand(time(NULL));  
    r = (int) (10*((double)rand())/ RAND_MAX);  
        
    printf("Num thread: %d, tid: %d\n",r,(int)pthread_self());
    
    pthread_mutex_lock (&mutex);  
    global += r; 
    pthread_mutex_unlock (&mutex);
    
    pthread_exit (NULL);
}

int main(void)
{
    pthread_t tid[N];
    int i,status;
    global = 0;
    
    for(i=0;i<N;i++)
    {
       
        if (pthread_create (&tid[i], NULL,f, NULL) != 0) {
            perror("pthread_create \n");
            exit (1);
        }
    }
    
    
    for (i=0; i < N; i++) 
    {
        if (pthread_join(tid[i], (void**) &status) != 0) {
            printf ("pthread_join"); 
            exit (1);
        }
        else
            printf ("Thread %d fini.\n",i);
    }

    printf("global: %d\n",global);
    
    return 0;
}

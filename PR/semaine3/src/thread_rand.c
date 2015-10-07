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

void *f(void* n)
{
    srand(time(NULL));
    
    global += (int) (10*((double)rand())/ RAND_MAX);
    
    printf("Num thread: %d, tid: %d\n",*n,(int)pthread_self());
    pthread_exit ((void*)&n);
}


int main(void)
{
    pthread_t tid[N];
    int i,status;
    int* pt_ind;
    global = 0;
    
    for(i=0;i<N;i++)
    {
        pt_ind = (int *) malloc (sizeof (i));
        *pt_ind =i;
        
        if (pthread_create (&tid[i], NULL,f, (void*)pt_ind) != 0) {
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
            printf ("Thread %d fini avec status :%d\n",i, status);
    }

    printf("global: %d\n",global));
    
    return 0;
}

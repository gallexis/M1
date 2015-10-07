#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string.h>

#include <pthread.h>

#define N 5

void *f(void* n)
{
    int* c = (int*) n;
    int *pt = malloc (sizeof (int) );
   
    printf("Num thread: %d, tid: %d\n",*c,(int)pthread_self());
        
    *c *= 2;    
    pt = (int*)c;
    
    pthread_exit ((void*)pt);
}


int main(void)
{
    pthread_t tid[N];
    int i;
    int *pt_ind, *status;
    
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
            printf ("Thread %d fini avec status :%d\n",i, *status);
    }

    
    return 0;
}

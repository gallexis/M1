#define _XOPEN_SOURCE 700
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include <pthread.h>
#include <signal.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t thread_creation = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t last_thread_created = PTHREAD_COND_INITIALIZER;
pthread_cond_t last_thread_terminated = PTHREAD_COND_INITIALIZER;
pthread_cond_t main_libere_thread = PTHREAD_COND_INITIALIZER;

int nbThreads;
int nbThreadsOrigine;

void *thread()
{
    pthread_mutex_lock(&m);
    
    
    if(--nbThreads == 0)
    {
        pthread_mutex_lock(&thread_creation);
        pthread_cond_signal(&last_thread_created);
        pthread_mutex_unlock(&thread_creation);
    }
    else
    {
        if (pthread_create (NULL, NULL, thread, (void*)0) != 0) {
            perror("pthread_create \n");
            exit (1);
        }
    }
    
    
    pthread_cond_wait(&main_libere_thread,&m);
    
    
    if(--nbThreadsOrigine == 0)
        pthread_cond_signal(&last_thread_terminated);
    pthread_mutex_unlock(&m);
    
    
    
    pthread_exit ((void*)0);
}


int main(int argc, char *argv[])
{
    pthread_t tid[1];
    int nbThreads;
    sigset_t ens;
    
    nbThreads = nbThreadsOrigine = atoi(argv[1]);
    
    if(argc < 2){
        printf("missing arg");
    }
    
    sigfillset(&ens);
    pthread_sigmask(SIG_SETMASK, &ens, NULL);
    
    tid[0] = 0;
    
    if (pthread_create (&tid[0], NULL, thread, (void*)0) != 0) {
        perror("pthread_create \n");
        exit (1);
    }
    
    /* On attend d'être réveillé par la dernière thread */
    pthread_mutex_lock(&thread_creation);
    pthread_cond_wait(&last_thread_created, &thread_creation);
    pthread_mutex_unlock(&thread_creation);
    
    
    printf("Tous mes descendants sont créés\n");
    
    /* sigint */
    
    
    
    pthread_cond_broadcast(&main_libere_thread);
    
    
  
    /* On attend d'être réveillé par la dernière thread */
    pthread_mutex_lock(&thread_creation);
    pthread_cond_wait(&last_thread_terminated, &thread_creation);
    pthread_mutex_unlock(&thread_creation);
    
    

return 0;
}

#define _XOPEN_SOURCE 700
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include <pthread.h>

void* thread_func(void* arg) {
    
    int i, nb;
    int *param;
    int *lvl = (int*)arg;
    pthread_t *tid;
    
    nb = (*lvl)+1;
    
    if (*lvl < max) {
        param = (int*)malloc(sizeof(int));
        *param = nb;
        tid = calloc(nb, sizeof(pthread_t));
        printf("%d cree %d fils\n", (int)pthread_self(), nb);
        for (i = 0; i < nb; i++) {
            pthread_create((tid+i), 0, thread_func, param);
        }
        for (i = 0; i < nb; i++)
            pthread_join(tid[i], NULL);
    }
    
    if (*lvl > 1)
        pthread_exit ( (void*)0);
    
    return (void*)0;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    int nbThreads;
    sigset_t sig_proc;
    
    nbThreads = atoi(argv[1]);
    
    if(argc < 2){
        printf("missing arg");
    }
    
    sigemptyset(&sig_proc);
    sigaddset (&sig_proc,SIGINT);
    sigprocmask (SIG_SETMASK, &sig_proc, NULL);
    
    
    if (pthread_create (&tid, NULL, thread_func, (void*)0) != 0) {
        perror("pthread_create \n");
        exit (1);
    }
    
    /* On attend d'être réveillé par la dernière thread */
    pthread_mutex_lock(&m);
    pthread_cond_wait(&last_thread_created, &m);
    pthread_mutex_unlock(&m);
    
    
    printf("Tous mes descendants sont créés\n");
    
    /* sigint */
    pause();
    
    
    pthread_cond_broadcast(&main_libere_thread);
    
    
    
    /* On attend d'être réveillé par la dernière thread */
    pthread_mutex_lock(&m);
    pthread_cond_wait(&last_thread_terminated, &m);
    pthread_mutex_unlock(&m);

    
    return 0;
}

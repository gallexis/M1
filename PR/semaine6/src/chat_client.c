#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <pthread.h>

void to_client(void *fd_server){
    
    while (read(fd_server,buff)) {
        fprintf(stdout,"%s",buff);
    }
    
    
}

void to_server(void *fd_client){
    
    while (read(stdin,buff)) {
        fprintf(fd_client,"%s",buff);
    }
    
}

int main(int argc, char *argv[])
{
    int i;
    int tmp;
    int fd_client;
    int fd_server;
    int *addr1;
    int *addr2;
    pthread_t tid1;
    pthread_t tid2;
    
    
    if(argc < 3){
        printf("missing arg");
    }
    
    if ((fd_client = shm_open(argv[1], O_RDWR|O_CREAT,0600)) == -1) {
        perror("shm_open");
        exit(1);
    }
    
    if ((fd_server= shm_open(argv[2], O_RDWR|O_CREAT,0600)) == -1) {
        perror("shm_open");
        exit(1);
    }
    
    
    if((addr1 = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd_client, 0))== MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    if((addr2 = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd_server, 0))== MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    

    /* CREATE */
    if (pthread_create (tid1, NULL, to_client, (void*)addr1) != 0) {
        perror("pthread_create \n");
        exit (1);
    }
    
    if (pthread_create (tid2, NULL, to_server, (void*)addr2) != 0) {
        perror("pthread_create \n");
        exit (1);
    }

    
    /* JOIN */
    if (pthread_join(tid1, (void**) &status) != 0) {
        printf ("pthread_join");
        exit (1);
    }
    else
        printf ("Thread 1 fini. \n");

    if (pthread_join(tid2, (void**) &status) != 0) {
        printf ("pthread_join");
        exit (1);
    }
    else
        printf ("Thread 2 fini. \n",i);

    
    shm_unlink(argv[1]);
    shm_unlink(argv[2]);
    
    printf("FIN \n");
    
    
    
    return 0;
}

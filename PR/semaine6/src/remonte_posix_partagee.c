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

#define MSG_SIZE 128

void remonte_ipc( int nb_fils, int *p_int){
    
    int compteur;
    int alea;
    pid_t pid;
    
    for(compteur=0;compteur<nb_fils;compteur++)
    {
        pid = fork();
        
        /* Fils */
        if(pid == 0)
        {
            alea = (int) (10*(float)rand()/ RAND_MAX);
            
            *(p_int+compteur) = alea;
            exit(0);
        }
        
        /* Pere */
        else if( pid > 0 )
        {
            
        }
        else
        {
            perror("Problème fork");
            exit(0);
        }
    }
    
    while ( waitpid(-1, NULL, 0) > 0 ) {
        printf("Fils terminé\n ");
    }
}

int main(int argc, char *argv[])
{
    int i;
    int tmp;
    int nb_fils;
    int fd;
    int *sp;
    
    tmp=0;
    
    if(argc < 2){
        printf("missing arg");
    }
    
    srand(time(NULL));
    
    nb_fils = atoi(argv[1]);    
    
    if ((fd = shm_open("/remonte_posix_partagee.c", O_RDWR|O_CREAT,0600)) == -1) {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(fd, nb_fils*sizeof(int)) == -1) {
        perror("ftruncate");
        exit(1);
    }
    
    if((sp = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0))== MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    remonte_ipc(nb_fils, sp);

    for (i=0; i<nb_fils; i++)
        tmp += *sp++;
       
    shm_unlink("/remonte_posix_partagee.c");
    
    printf("somme alea: %d\n", tmp);
    
    
    return 0;
}

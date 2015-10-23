#define SVID_SOURCE 1
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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
    key_t cle;
    int shm_id;
    int *p_int;
    int *adr_att;
    int taille;
    
    tmp=0;
    
    if(argc < 2){
        printf("missing arg");
    }
    
    srand(time(NULL));
    
    nb_fils = atoi(argv[1]);
    taille = nb_fils*sizeof(int);
    
    cle = ftok("remonte_partagee.c"  , getpid() & 0xFF);
    shm_id = shmget(cle, taille, 0666 | IPC_CREAT);
    adr_att = shmat(shm_id, 0, 0600);
    
    p_int = (int *)adr_att;
    
    remonte_ipc(nb_fils, p_int);
    
    p_int = (int *)adr_att;
    
    for (i=0; i<nb_fils; i++)
        tmp += *p_int++;
    
    
    shmdt(adr_att);
    shmctl(shm_id, IPC_RMID, (struct shmid_ds *) NULL);
    
    printf("somme alea: %d\n", tmp);
    
    
    return 0;
}

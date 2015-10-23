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
#include<sys/msg.h>

#define MAX_FILS 128

struct message {
    long type;
    int pid;
    int message;
}msg;

void remonte_ipc( int nb_fils, int file)
{
    int i;
    int j;
    int compteur;
    int alea;
    pid_t pid;
    int tmp;
    
    tmp =0;
    
    for(compteur=0;compteur<nb_fils;compteur++)
    {
        pid = fork();
        
        /* Fils */
        if(pid == 0)
        {
            alea = (int) (10*(float)rand()/ RAND_MAX)+1;
            
            msg.type = getppid();
            msg.pid = getpid();
            msg.message = alea;
            
            msgsnd(file , &msg , 2*sizeof(int)+sizeof(long), 0);
            
            for (i=0;i<alea;i++ )
            {
                msgrcv(file , &msg , sizeof(int), getpid(),0);
                tmp += msg.message;
            }
            
            printf("Fils: %d - somme alea: %d\n", getpid(),tmp);
            
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
    
    for(i=0;i<nb_fils;i++)
    {
        msgrcv(file , &msg , 2*sizeof(int)+sizeof(long), getpid(),0);
        
        alea = msg.message;
        
        for(j=0;j<alea;j++)
        {
            msg.type = msg.pid;
            msg.message =  (int) (100*(float)rand()/ RAND_MAX);
            msgsnd(file , &msg , sizeof(int), 0);
        }
    }
    
    while ( waitpid(-1, NULL, 0) > 0 ) {
        /* printf("Fils terminé\n "); */
    }
}

int main(int argc, char *argv[])
{
    int tmp;
    int nb_fils;
    key_t cle;
    int file;
    
    srand(time(NULL));
    if(argc < 2){
        printf("missing arg");
    }
    tmp=0;
    
    nb_fils = atoi(argv[1]);
    
    cle = ftok("multi_remonte_file_unique.c" ,getpid() & 0xFF);
    file = msgget (cle, 0666 | IPC_CREAT);
    
    remonte_ipc(nb_fils, file);
    printf("prog terminé\n ");
    
    msgctl(file, IPC_RMID, (struct msqid_ds *) NULL);
    
    return 0;
}

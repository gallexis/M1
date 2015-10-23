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

#define MSG_SIZE 128

struct message {
    long type;
    int alea;
} msg;

void remonte_ipc( int nb_fils,int msg_id){
    
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
            msg.alea = alea;
            msg.type = 1;
            
            msgsnd(msg_id , &msg , MSG_SIZE, 0);
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
    int msg_id;
    /* struct msqid_ds *buf; */
    
    tmp=0;
    
    if(argc < 2){
        printf("missing arg");
    }
    
    srand(time(NULL));
    
    nb_fils = atoi(argv[1]);
    
    cle = ftok("remonte_ipc.c"  , getpid() & 0xFF);
    msg_id = msgget (cle, 0666 | IPC_CREAT);
    
    remonte_ipc(nb_fils, msg_id);
    
    for (i=0;i<nb_fils;i++ ) {
        msgrcv(msg_id , &msg , MSG_SIZE, 1L,0);
        tmp += msg.alea;
    }
    
    msgctl(msg_id, IPC_RMID, (struct msqid_ds *) NULL);
    
    printf("somme alea: %d\n", tmp);
    return 0;
}

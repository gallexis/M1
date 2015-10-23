#define _SVID_SOURCE 1
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
    int message;
    int msg_id_fils;
}msg;

void remonte_ipc( int nb_fils, int msg_id_pere)
{
    int i;
    int j;
    int compteur;
    int alea;
    pid_t pid;
    key_t cle;
    int msg_id_fils;
    int tmp;
    
    tmp =0;
    msg_id_fils = 0;
    
    for(compteur=0;compteur<nb_fils;compteur++)
    {
        pid = fork();
        
        /* Fils */
        if(pid == 0)
        {
            cle = ftok("multi_remonte.c" ,getpid() & 0xFF);
            msg_id_fils = msgget (cle, 0666 | IPC_CREAT);
            
            alea = (int) (10*(float)rand()/ RAND_MAX)+1;
            
            msg.type = 1;
            msg.message = alea;
            msg.msg_id_fils = msg_id_fils;
            
            msgsnd(msg_id_pere , &msg , sizeof(int), 0);
            
            for (i=0;i<alea;i++ )
            {
                msgrcv(msg_id_fils , &msg , sizeof(int), 1L,0);
                tmp += msg.message;
            }

            printf("Fils: %d - somme alea: %d\n", getpid(),tmp);
            
            msgctl(msg_id_fils, IPC_RMID, (struct msqid_ds *) NULL);
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
    
    for (i=0;i<nb_fils;i++ )
    {
        msgrcv(msg_id_pere , &msg , sizeof(int), 1L,0);
        
        alea = msg.message;
        msg_id_fils = msg.msg_id_fils;
        
        
        for(j=0;j<alea;j++)
        {
            msg.type = 1;
            msg.message =  (int) (100*(float)rand()/ RAND_MAX);
            msgsnd(msg_id_fils , &msg , sizeof(int), 0);
        }
    }
    
    while ( waitpid(-1, NULL, 0) > 0 ) {
        /* printf("Fils terminé\n "); */
    }
}

int main(int argc, char *argv[])
{
    int nb_fils;
    key_t cle;
    int msg_id_pere;
    
    /* srand(time(NULL)); */
    if(argc < 2){
        printf("missing arg");
    }

    
    nb_fils = atoi(argv[1]);
    
    cle = ftok("multi_remonte.c" ,getpid() & 0xFF);
    msg_id_pere = msgget (cle, 0666 | IPC_CREAT);
    
    
    remonte_ipc(nb_fils, msg_id_pere);
    printf("prog terminé\n ");

    
    msgctl(msg_id_pere, IPC_RMID, (struct msqid_ds *) NULL);
    
    
    return 0;
}

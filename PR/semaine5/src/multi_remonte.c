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

struct messageFils {
    int message;
};

struct messagePere {
    int nbMessagesMax;
    struct messageFils *fils;
};

void remonte_ipc( int nb_fils, struct messagePere *pere,int msg_id_pere)
{
    int i;
    int j;
    int compteur;
    int alea;
    pid_t pid;
    key_t cle;
    int msg_id_fils;
    struct messageFils fils;
    int tmp;
    
    tmp =0;
    
    
    
    for(compteur=0;compteur<nb_fils;compteur++)
    {
        pid = fork();
        
        /* Fils */
        if(pid == 0)
        {
            cle = ftok(".", 'Q');
            msg_id_fils = msgget (cle, 0666 | IPC_CREAT);
            
            /* BUG */
            alea = (int) (10*(float)rand()/ RAND_MAX);
            pere->nbMessagesMax = alea;
            pere->fils = &fils;
            
            msgsnd(msg_id_pere , pere , sizeof(struct messagePere), 0);
            
            
            for (i=0;i<alea;i++ ) {
                msgrcv(msg_id_fils , &fils , sizeof(struct messageFils), 1L,0);
                tmp += fils.message;
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
        msgrcv(msg_id_pere , &pere , sizeof(struct messagePere), 1L,0);
        
        alea = pere->nbMessagesMax;
        for(j=0;j<alea;j++)
        {
            pere->fils->message =  (int) (100*(float)rand()/ RAND_MAX);
        }
    }
    
    while ( waitpid(-1, NULL, 0) > 0 ) {
        printf("Fils terminé\n ");
    }
}

int main(int argc, char *argv[])
{
    int tmp;
    int nb_fils;
    key_t cle;
    int msg_id_pere;
    struct messagePere pere;
    
    /* srand(time(NULL)); */
    if(argc < 2){
        printf("missing arg");
    }
    tmp=0;
    
    nb_fils = atoi(argv[1]);
    
    cle = ftok("/tmp", 'Q');
    msg_id_pere = msgget (cle, 0666 | IPC_CREAT);
    
    remonte_ipc(nb_fils, &pere, msg_id_pere);
    

    
    msgctl(msg_id_pere, IPC_RMID, (struct msqid_ds *) NULL);
    
    
    return 0;
}

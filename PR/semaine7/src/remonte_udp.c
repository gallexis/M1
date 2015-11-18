#define _SVID_SOURCE 1
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>

#define MSG_SIZE 128

void remonte_sock( int nb_fils, int sock){
    
    int compteur;
    int alea;
    char message[1];
    pid_t pid;
    
    for(compteur=0;compteur<nb_fils;compteur++)
    {
        pid = fork();
        
        /* Fils */
        if(pid == 0)
        {
            alea = (int) (10*(float)rand()/ RAND_MAX);
            message[0] = alea + '0'
          
            if (write(sock, message, strlen(message)+1) == -1) {
                perror("write ");
                exit(1);
            }
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
    int sock;
    struct sockaddr_un addr;
    char message[1];
    
    tmp=0;
    
    if(argc < 2){
        printf("missing arg");
    }
    
    srand(time(NULL));
    
    nb_fils = atoi(argv[1]);
    taille = nb_fils*sizeof(int);
   
    

    memset(&addr,’\0’, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "./MySock");
    
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("Erreur creation socket");
        exit(1);
    }
    
    if (bind(sock, (struct sockaddr *)&addr, strlen(addr.sa_data)) == -1)
    {
        perror("Erreur au nommage");
        exit(2);
    }

    
    remonte_sock(nb_fils, sock);
    
    /*** Reception du message ***/
    while (i<nb_fils) {
        if ( read(sock, message, strlen(message)+1) == -1) {
            perror("recvfrom"); exit(2);
        }
        tmp += atoi(message[0])
        
        i++;
    }
    
    close(sock);

    printf("somme alea: %d\n", tmp);
    unlink(addr.sun_path);
    
    return 0;
}

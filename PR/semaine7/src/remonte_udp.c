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
#include <sys/un.h>
#include <string.h>

#define MSG_SIZE 128

void remonte_sock( int nb_fils, int sock,struct sockaddr_un addr){
    
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
            message[0] = alea + '0';
        
            if (sendto(sock,message,strlen(message)+1,0,(struct sockaddr_un *)&addr, sizeof(addr)) == -1) {
                perror("sendto"); exit(1);
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

}

int main(int argc, char *argv[])
{
    int i;
    int tmp;
    int nb_fils;
    int sock;
    struct sockaddr_un addr;
    char message[1];
    socklen_t t =sizeof addr;
    tmp=i=0;
    
    if(argc < 3){
        printf("missing arg");
    }
    
    unlink(argv[1]);
    srand(time(NULL));
    
    nb_fils = atoi(argv[2]);

    memset(&addr,"\0", sizeof(const struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, argv[1]);
    
    if ((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("Erreur creation socket");
        exit(1);
    }
    
    if (bind(sock, (const struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Erreur au nommage");
        exit(2);
    }

    
    remonte_sock(nb_fils, sock, addr);
    
    /*** Reception du message ***/
    while (i<nb_fils) {
        if ( recvfrom(sock,&message[0],strlen(message)+1,0,(struct sockaddr *)&addr,&t) == -1) {
            perror("recvfrom"); exit(2);
        }

        tmp += atoi(&message[0]);
        
        i++;
    }
    
    
    while ( waitpid(-1, NULL, 0) > 0 ) {
        printf("Fils terminé\n ");
    }
    
    close(sock);

    printf("somme alea: %d\n", tmp);
    unlink(addr.sun_path);
    
    return 0;
}

#define _SVID_SOURCE 1
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>




#define MSG_SIZE 128

void manage_requests( char* req, FILE *fd)
{
    fwrite(req , strlen(req) , sizeof(req) , fd );
}

int main(int argc, char *argv[])
{
    int tmp;
    int port;
    int sock;
    struct sockaddr_in sin;
    struct sockaddr_in exp;
    int fromlen;
    char message[80];
    int cpt = 0;
    FILE *fd;
    fd = NULL;
    
    fromlen = sizeof(exp);
    tmp=0;
    
    if(argc < 2){
        printf("missing arg");
    }
    
    port = atoi(argv[1]);
    

    /* creation de la socket */
    if ((sock = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
        perror("socket");exit(1);
    }
    
    /* remplir le « nom » */
    memset((char *)&sin,0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);
    sin.sin_family = AF_INET;
    
    /* nommage */
    if (bind(sock,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
        perror("bind"); exit(2);
    }
    
    while (1) {
        /* Reception du message */
        if ( recvfrom(sock,message,sizeof(message),0,(struct sockaddr *)&exp,(socklen_t *)&fromlen) == -1) {
            perror("recvfrom"); exit(2);
        }
    
         printf("Message : %s \n", message); 
        
        if (cpt==0) {
            if ((fd = fopen (message, "w")) == NULL) {
                perror("Erreur de l'ouverture du fichier (fopen).\n");
                exit(EXIT_FAILURE);
            }
            cpt++;
        }
        
        manage_requests(message,fd);
        
        /*** Envoyer la reponse ***/
        if (sendto(sock,&cpt,sizeof(cpt),0,(struct sockaddr *)&exp,fromlen) == -1) {
            perror("sendto"); exit(4);
        }
        
        if (strlen(message) == 0) {
            break;
        }

    }
    
    close(sock);
    
    
    return 0;
}

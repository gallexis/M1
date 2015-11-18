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

void manage_requests( char* req, int sock,struct sockaddr exp)
{
    char *rep;
    rep = "ok";
    
     /* parse requete, get, get */
    

    
    /*** Envoyer la reponse ***/
    if (sendto(sock,rep,strlen(rep)+1,0,(struct sockaddr *)&exp,sizeof(exp)) == -1) {
        perror("sendto"); exit(4);
    }

}

int main(int argc, char *argv[])
{
    int port;
    int sock;
    struct sockaddr_in sin;
    struct sockaddr_in exp;
    int sock ;
    int fromlen;
    char message[80];
    int cpt = 0;
    
    fromlen = sizeof(exp);
    
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
        if ( recvfrom(sc,message,sizeof(message),0,(struct sockaddr *)&exp,&fromlen) == -1) {
            perror("recvfrom"); exit(2);
        }
        
        /* printf("Message : %s \n", message); */
        
        manage_requests(message, sock, exp);
        
    }
    
    close(sock);
    return 0;
}

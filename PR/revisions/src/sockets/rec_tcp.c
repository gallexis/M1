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
#define PORTSERV 1337

struct sockaddr_in serveur;
struct sockaddr_in exp;
int sock_server;
int sock_client;

int main(int argc, char *argv[])
{
    char message[255];
    socklen_t size_of_exp;
    
    size_of_exp = sizeof(exp);
    
    if(argc < 3){
        printf("missing arg");
    }
    
    /* memset(&addr,"\0", sizeof(const struct sockaddr_in)); */
    
    serveur.sin_family = AF_INET;
    serveur.sin_port = htons(PORTSERV);
    serveur.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if ((sock_server = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Erreur creation socket");
        exit(1);
    }
    
    if (bind(sock_server, (const struct sockaddr *)&serveur, sizeof(struct sockaddr)) == -1)
    {
        perror("Erreur au nommage");
        exit(2);
    }
    
    listen(sock_server, 5);
    
    /* boucle infinie */
    while( (sock_client = accept(sock_server, (struct sockaddr *)&exp, &size_of_exp)) )
    {
        printf("Connection accepted");

        recv(sock_client,message,sizeof(message),0);
        
        printf("Message : %s \n", message);
    }

    close(sock_client);
    close(sock_server);
    return 0;
}

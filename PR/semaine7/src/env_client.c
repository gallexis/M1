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

#define SIZE_LINE 256

void send_to_server( int sock, char *buff, struct sockaddr dest)
{
    /* parse buff */
    
    if (sendto(sock,parsedBuff,strlen(line)+1,0,(struct sockaddr *)&dest, sizeof(dest)) == -1) {
        perror("sendto"); exit(1);
    }
    
}

int main(int argc, char *argv[])
{
    int n;
    int port;
    int sock;
    struct sockaddr_in dest;
    int fromlen;
    char message[80];
    char buff[80];
    struct addrinfo *result;
    FILE *fd;
    fd = NULL;
    
    fromlen = sizeof(exp);
    n=0;
    
    if(argc < 4){
        printf("missing arg");
    }
    
    port = atoi(argv[2]);
    
    
    
    if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
        perror("socket"); exit(1);
    }
    
    /* creation de la socket */
    if ((sock = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
        perror("socket");exit(1);
    }
    
    /* Remplir la structure dest */
    if (getaddrinfo(argv[1], 0, 0, &result) != 0) {
        perror("getaddrinfo"); exit(EXIT_FAILURE);
    }
    
    memset((char *)&dest,0, sizeof(dest));
    memcpy((void*)&((struct sockaddr_in*)result->ai_addr)->sin_addr, (void*)&dest.sin_addr, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORTSERV);
    
    while((n = read(stdin, buff, 256)) > 0)
        send_to_server(sock,buff,dest);
    
    /* Envoyer le message
     if (sendto(sock,message,strlen(message)+1,0,(struct sockaddr *)&dest, sizeof(dest)) == -1) {
     perror("sendto"); exit(1);
     }
     */
    
    
    close(sock);
    return 0;
}

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

struct sockaddr_in addr;
struct sockaddr_in exp;

int main(int argc, char *argv[])
{
    int sock;
    char message[1];
    socklen_t size_of_addr;
    socklen_t size_of_exp;
    
    size_of_addr = sizeof(addr);
    size_of_exp = sizeof(exp);
    
    if(argc < 3){
        printf("missing arg");
    }
    
    /* memset(&addr,"\0", sizeof(const struct sockaddr_in)); */
    
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORTSERV);
    dest.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Erreur creation socket");
        exit(1);
    }
    
    if (bind(sock, (const struct sockaddr *)&addr, size_of_addr) == -1)
    {
        perror("Erreur au nommage");
        exit(2);
    }
    
    if ( recvfrom(sock,message,sizeof(message),0,(struct sockaddr *)&exp,(socklen_t *)&size_of_exp) == -1) {
        perror("recvfrom"); exit(2);
    }
    
    printf("Message : %s \n", message);
    
    
    close(sock);
    
    return 0;
}

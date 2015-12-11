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

struct sockaddr_in dest;
int sock;

int main(int argc, char *argv[])
{
    socklen_t size_of_dest = sizeof dest;
    char message[1];

    if(argc < 2){
        printf("missing arg");
    }
    
    /* memset(&addr,"\0", sizeof(const struct sockaddr_in)); */
    
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORTSERV);
    dest.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Erreur creation socket");
        exit(1);
    }
    
    message[0] = 'a';
    
    if (sendto(sock,message,strlen(message)+1,0,(struct sockaddr_in *)&dest, size_of_dest) == -1)
    {
        perror("sendto");
        exit(1);
    }
    
    close(sock);
    return 0;
}

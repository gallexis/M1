#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SIZE_MSG 100

int main(int argc, const char **argv)
{
    struct sockaddr_in dest;
    int sock;
    int fromlen = sizeof(dest);
    char message[SIZE_MSG];
    char reponse[SIZE_MSG];;
    int PORTSERV=atoi(argv[2]);
    if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
        perror("socket");
        exit(1);
    }
    
    memset((char *)&dest,0, sizeof(dest));
    
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORTSERV);
    dest.sin_addr.s_addr = inet_addr(argv[1]);
    
    
    while((fgets(message,SIZE_MSG,stdin))!=NULL)
    {
        if(strlen(message)<=1) break;
        printf("msg: %d\n",strlen(message));
        if (sendto(sock,message,SIZE_MSG,0, (struct sockaddr *)&dest, sizeof(dest)) == -1) {
            perror("sendto"); exit(1);
        }
        
        if (recvfrom(sock,reponse,SIZE_MSG,0,0,&fromlen) == -1) {
            perror("recvfrom"); exit(1);
        }
        printf("Reponse: %s\n",reponse);
    }
    
    close(sock);
    return(0);
}

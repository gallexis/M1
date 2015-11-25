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

#define SIZE_LINE 256

void send_to_client( int sock, char *path, struct sockaddr_in dest)
{
    char line[SIZE_LINE];
    FILE *fp;
    fp = fopen( path , "r" );
    
    if (sendto(sock,path,strlen(path)+1,0,(struct sockaddr *)&dest, sizeof(dest)) == -1) {
        perror("sendto"); exit(1);
    }
    
    while (!feof(fp)) {
        fgets(line,SIZE_LINE,fp);
        
        if (sendto(sock,line,strlen(line)+1,0,(struct sockaddr *)&dest, sizeof(dest)) == -1) {
            perror("sendto"); exit(1);
        }
    }

}

int main(int argc, char *argv[])
{
    int tmp;
    int port;
    int sock;
    struct sockaddr_in dest;
    int fromlen;
    struct addrinfo *result;
    FILE *fd;
    fd = NULL;
    
    fromlen = sizeof(exp);
    tmp=0;
    
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
    dest.sin_port = htons(port);
    
    send_to_client(sock,argv[3],dest);
    
    /* Envoyer le message
    if (sendto(sock,message,strlen(message)+1,0,(struct sockaddr *)&dest, sizeof(dest)) == -1) {
        perror("sendto"); exit(1);
    }
     */
    
    
    close(sock);
    return 0;
}

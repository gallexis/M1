#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SIZEMSG 100
#define MAXCLIENT 5

struct msg{
    char op;
    char ident[10];
    char value[10];
} mymsg;

int main(int argc, const char **argv)
{
    int PORTSERV=atoi(argv[1]);
    struct sockaddr_in sin;
    int sc;
    int i;
    int p;
    int scom;
    struct sockaddr_in exp;
    int fromlen = sizeof exp;
    char message[SIZEMSG];
    
    /* creation de la socket */
    if ((sc = socket(AF_INET,SOCK_DGRAM,0)) < 0) {
        perror("socket"); exit(1);
    }
    memset((char *)&sin,0, sizeof(sin));
    
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORTSERV);
    sin.sin_family = AF_INET;
    
    /* nommage */
    if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
        perror("bind");
        exit(1);
    }

    if((p=fork())==0)
    {
        while(1)
        {
            printf("waiting \n");
            if ( recvfrom(sc,message,SIZEMSG,0,(struct sockaddr *)&exp,&fromlen) == -1) {
                perror("recvfrom"); exit(2);
            }
            printf("%s\n",message );
            sscanf(message,"%c %s %s",&mymsg.op,&mymsg.ident,&mymsg.value);
            printf("%c\n",mymsg.op );
            printf("%s\n",mymsg.ident);
            printf("%s\n",mymsg.value);
            
            if(mymsg.op=='Q')
            {
                strcpy(message,"Au revoir");
                if (sendto(sc,message,SIZEMSG,0,(struct sockaddr *)&exp,fromlen) == -1) {
                    perror("sendto"); exit(4);
                }
                break;
            }
            
            if(mymsg.op=='S')
            {
                printf("op s \n");
                setenv(mymsg.ident,mymsg.value,1);
                strcpy(message,"Variable ajoute");
                if (sendto(sc,message,SIZEMSG,0,(struct sockaddr *)&exp,fromlen) == -1) {
                    perror("sendto"); exit(4);
                }
            }
            
            if(mymsg.op=='G')
            {
                if((getenv(mymsg.ident))==NULL)
                {
                    /*Variable inexistante*/
                    strcpy(message,"Variable inexistante");
                    if (sendto(sc,message,SIZEMSG,0,(struct sockaddr *)&exp,fromlen) == -1) {
                        perror("sendto"); exit(4);
                    }
                }
                else{
                    strcpy(message,getenv(mymsg.ident));
                    if (sendto(sc,message,SIZEMSG,0,(struct sockaddr *)&exp,fromlen) == -1) {
                        perror("sendto"); exit(4);
                    }
                }
            }
        }
      
    }
    wait(NULL);
    close(sc);
    return (0);
}

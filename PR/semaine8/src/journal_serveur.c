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


int main(int argc, char *argv[])
{
    struct sockaddr_in sin;
    int *sc ;
    int scom;
    struct hostent *hp;
    int fromlen = sizeof exp;
    int cpt;
    struct timeval tv;
    sc = malloc(argc * sizeof(int));
    
    socklen_t len;
    struct sockaddr_storage addr;
    char ipstr[INET6_ADDRSTRLEN];
    int port;
    char infos[100];
        
    fd_set mselect;
    FD_ZERO(&mselect);
    
    /* Wait up to 2 seconds. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    
    for(i =0;i< argc;i++){
        
        if ((sc[i] = socket(AF_INET,SOCK_STREAM,0)) < 0) {
            perror("socket"); exit(1);
        }
        memset((char *)&sin,0, sizeof(sin));
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        sin.sin_port = htons(PORTSERV);
        sin.sin_family = AF_INET;
        
        if (bind(sc[i],(struct sockaddr *)&sin,sizeof(sin)) < 0) {
            perror("bind");
            exit(1);
        }
        listen(sc[i], 5);
        
        
        FD_SET(sc[i],&mselect);
    }
    
    FILE *fp;
    fp = fopen( "cx.log" , "w" );
    
    /* Boucle principale */
    for (;;) {
        
        //                +1 ?
        if (select(sc+argc, &mselect, NULL, NULL, &tv) == -1) {
            perror("select");
            exit(3);
        }
        
        for(i =0;i< argc;i++)
        {
            // ou FD_ISSET(i, &mselect) ??
            if (FD_ISSET(sc[i], &mselect))
            {
                if ( (scom = accept(sc[i], (struct sockaddr *)&exp, &fromlen))== -1) {
                    perror("accept"); exit(2);
                }
                
                len = sizeof addr;
                
                struct sockaddr_in *s = (struct sockaddr_in *)&exp;
                port = ntohs(s->sin_port);
                
                inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
                
                strcpy(infos,ipstr);
                strcpy(infos,":");
                strcpy(infos,port+'0');
                
                fwrite(infos , strlen(infos) , sizeof(infos) , fd );
                
                
                /* Fermer la connexion */
                close(sc[i]);
                close(scom);
                
            }
        }
        break
    }
}


return 0;
}


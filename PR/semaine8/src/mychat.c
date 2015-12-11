#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>

struct ip_mreq
{
    struct in_addr imr_multiaddr;	/* IP multicast address of group */
    struct in_addr imr_interface;	/* local IP address of interface */
};

int PORTSERV;
int sock;
struct sockaddr_in sin;
char adresse[20];
char ident[10];

void send_message()
{
struct sockaddr_in dest;
memset((char *)&dest, 0, sizeof(dest));
dest.sin_addr.s_addr = inet_addr("225.0.0.10");
dest.sin_family = AF_INET;
dest.sin_port = htons(PORTSERV);
int j=0;
/*if(setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &j,sizeof(j)) == -1){
perror("setsockopt");
exit(2);
}*/
  char message[180];
  while (1) {
    int t=sizeof(dest);
    printf("Entrer un message\n");
    scanf("%s",message );
    strcat(message,":  ");
    strcat(message,ident);
    if ( sendto(sock,message,180,0, (struct sockaddr*)&sin, t) == -1) {
    perror("sendto"); exit(1);
    }
    printf("msg send\n");
  }

}

void read_message()
{
  /* Reception du message */
  char msg[180];
  int k=sizeof(sin);
  while(1)
  {
    printf("waiting \n");
    if ((recvfrom(sock,msg,180,0,NULL,NULL)) == -1) {
    perror("recvfrom");
    exit(4);
    }
    printf("message: %s\n",msg);
  }
}

int main(int argc, const char **argv)
{
  int N=argc-1;
  pthread_t send;
  pthread_t recv;
  strcpy(adresse,argv[1]);
  strcpy(ident,argv[3]);
  PORTSERV=atoi(argv[2]);
  if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
  perror("socket");
  exit(1);
  }
  struct ip_mreq imr;
  /* remplir le « nom » */

  imr.imr_multiaddr.s_addr = inet_addr(adresse);
  imr.imr_interface.s_addr = INADDR_ANY;
  if((setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,&imr,  sizeof(struct ip_mreq))) == -1){
  perror("sets ockopt");
  exit(2);
  }
  memset((char *)&sin,0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr =htonl(INADDR_ANY);
  sin.sin_port = htons(PORTSERV);

  /* nommage */
  if (bind(sock,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
  perror("bind");
  }
  if(pthread_create(&recv,NULL,read_message,(void*)(0))!=0)
			{
				perror("erreur creation");
				exit(0);
			}

  if(pthread_create(&send,NULL,send_message,(void*)(0))!=0)
			{
				perror("erreur creation");
				exit(0);
			}

      if(pthread_join(send,NULL)!=0)
      {
        perror("erreur terminaison");
        exit(0);
      }
      if(pthread_join(recv,NULL)!=0)
      {
        perror("erreur terminaison");
        exit(0);
      }
return (0);
}

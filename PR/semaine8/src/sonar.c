#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define PORTSERV 9999
int sock;
struct sockaddr_in sin;

void * send_message()
{
  while (1) {
    int t=sizeof(sin);
    int h=sendto(sock,"PING",strlen("PING")+1,0, (struct sockaddr*)&sin,t);
    if ( h == -1) {
    perror("sendto"); exit(1);
    }
    printf("msg send\n");
  sleep(3);
  }

}

void * read_message()
{
  /* Reception du message */
  char message[80];
  int k=sizeof(sin);
  while(1)
  {
    if ((recvfrom(sock,message,80,0,NULL,NULL)) == -1) {
    perror("recvfrom");
    exit(4);
    }
    printf("message: %s\n",message);
}
}

int main(int argc, const char **argv)
{
  int N=argc-1;
  pthread_t send;
  pthread_t recv;
  if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
  perror("socket");
  exit(1);
  }
  /*imr.imr_multiaddr.s_addr = inet_addr(INADDR_BROADCAST);
  imr.imr_interface.s_addr = INADDR_ANY;*/
  int g=1;
  if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &g, sizeof(g)) == -1){
  perror("setsockopt");
  exit(2);
  }
  /* remplir le « nom » */
  memset((char *)&sin,0, sizeof(sin));
  sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);
  sin.sin_port = htons(PORTSERV);
  sin.sin_family = AF_INET;
  /* nommage */
  if (bind(sock,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
  perror("bind");
  exit(3);
  }
  if(pthread_create(&send,NULL,send_message,(void*)(0))!=0)
			{
				perror("erreur creation");
				exit(0);
			}

  if(pthread_create(&recv,NULL,read_message,(void*)(0))!=0)
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

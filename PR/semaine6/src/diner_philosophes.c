#define _SVID_SOURCE 1
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include<sys/msg.h>

#include <semaphore.h>


/*

il peut y avoir un dead lock, tous les philosophes attendent qu'une baguette se libere
sauf qu'elles sont toutes utilisées


*/

void philosophes( int N,int C,int P,int M){
    
    int compteur;
    pid_t pid;
    sem_t *b;
    sem_t p;
    int g;
    int d;
    int i;
    
    g = 0;
    d = 0;
    i=0;
    
    sem_init(&p, 1, N-1);
    
    b = malloc(N*sizeof(int));
    for(i=0;i<N;i++)
        sem_init(&b[i], 1, 1);
    
    for(compteur=0;compteur<N;compteur++)
    {
        pid = fork();
        
        /* Fils */
        if(pid == 0)
        {           
            for(i=0;i<C;i++)
            {            
              
                sem_wait(&p); 
                                     
                g = (compteur-1) % N;
                if(g==-1)
                    g=N-1;
                    
                d = (compteur+1) % N;
                
                sleep(P);
                                
                sem_wait(&b[g]); 
                sleep(1);
                sem_wait(&b[d]); 
                
                
                printf("+Philosophe %d prend baguette\n",compteur);
                  
                sleep(M);
                                    
                sem_post(&b[g]);
                sem_post(&b[d]);
                
                printf("-Philosophe %d relache baguette\n",compteur);
                sem_post(&p);
            } 

            exit(compteur);
        }
        
        /* Pere */
        else if( pid > 0 )
        {
            
        }
        else
        {
            perror("Problème fork");
            exit(0);
        }
    }
    
    for(i=0;i<N;i++){
       pid = wait(NULL);
        printf("Fils terminé %d \n ",pid);
    }
}

int main(int argc, char *argv[])
{
    int N;
    int C;
    int P;
    int M;

    if(argc < 5){
        printf("missing arg");
    }

    N = atoi(argv[1]); 
    C = atoi(argv[2]); 
    P = atoi(argv[3]); 
    M = atoi(argv[4]); 
   

    philosophes(N,C,P,M);
      
   
    return 0;
}

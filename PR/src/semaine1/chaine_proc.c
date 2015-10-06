#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void chaineProc( int nb_fils){
    
    int compteur,i, status;
    pid_t pids[nb_fils], pid, pidfils;
    
    for(compteur=0;compteur<nb_fils;compteur++)
    {
        pid = fork();
        
        /* Fils */
        if(pid == 0)
        {
            pids[compteur] = getppid();
            
            if(compteur == nb_fils-1)
            {
                for (i = 0; i < nb_fils; i++) {
                    printf("pid n° %d : %d\n",i,pids[i]);
                }
                
                exit((int)(rand () /(((double) RAND_MAX +1) /100)));
            }
        }
        
        /* Pere */
        else if( pid > 0 )
        {
            pidfils = wait(&status);
            
            if (WIFEXITED (status) ) {
                printf("compteur: %d ,pid pere: %d, pid fils: %d, mon pid: %d, status: %d\n",compteur,getppid(),pidfils,getpid(),WEXITSTATUS (status));
            }
            
            exit(WEXITSTATUS (status));
        }
        
        else
        {
            perror("Problème fork");
            exit(0);
        }
    }
    
}

int main(){
    srand(time(NULL));
    chaineProc(3);
    return 0;
}
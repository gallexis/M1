#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void fkill( int nb_fils){
    
    int compteur,i, status;
    pid_t pids[nb_fils], pid, pidfils;
    
    sigset_t sig_proc;
    
    
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
                
                kill(getppid(), SIGINT);
                printf("mort %d\n",getpid());
                exit(0);
            }
        }
        
        /* Pere */
        else if( pid > 0 )
        {
            printf("Pere %d attend mort fils %d\n",getpid(), pid);
            
            pause();
            
            printf("fils mort %d \n",pid);
            
            if(compteur > 0)
                kill(getppid(), SIGINT);
            
            exit(0);
            
        }
        
        else
        {
            perror("Problème");
            exit(0);
        }
    }
    
}

int main(){
    
    fkill(3);
    return 0;
}

#define _POSIX_SOURCE 1
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void P3created(int sig){
    printf ("P3 created \n");
}

void P3destroyed(int sig){
    printf ("P3 destroyed \n");
}

void synchro(int N){
    
    int compteur,status;
    pid_t pids[N], pid, pidfils;
    
    sigset_t sig_proc1,sig_proc2;
    struct sigaction action1,action2;
    
    for(compteur=1;compteur<=N;compteur++)
    {
        pid = fork();
        pids[compteur-1]=pid;
        
        /* Fils */
        if(pid == 0)
        {
            if(compteur==3)
            {
                kill (pids[0], SIGUSR1);
                exit(EXIT_SUCCESS);
            }
        }
        
        /* Pere */
        else if( pid > 0 )
        {
            if(compteur==1)
            {
                sigemptyset(&sig_proc1);
                action1.sa_mask=sig_proc1;
                action1.sa_flags=0;
                action1.sa_handler = P3created;
                
                
                sigemptyset(&sig_proc2);
                action2.sa_mask=sig_proc2;
                action2.sa_flags=0;
                action2.sa_handler = P3destroyed;
                
                
                sigaction(SIGUSR1, &action1,0);
                sigaction(SIGUSR2, &action2,0);
                
                
                pidfils = wait(&status);
                
                if (WIFEXITED (status)) {
                    printf("P2 terminé");
                }
                else
                {
                    perror("Problème terminaison fils");
                    printf("pb fils: pid_pere: %d\n",getpid());
                }
                
            }
            
            if(compteur==2)
            {
                pidfils = wait(&status);
                
                if (WIFEXITED (status)) {
                    kill (getppid(), SIGUSR2);
                    exit(EXIT_SUCCESS);
                }
                else
                {
                    perror("Problème terminaison fils");
                    printf("pb fils: pid_pere: %d\n",getpid());
                }
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
    synchro(3);
    return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void fkill( int nb_fils){
    
    int compteur,i, status;
    pid_t pids[nb_fils], pid, pidfils;
    
    sigset_t sig_proc,sig_proc2;
    
    
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
                kill(pids[0], SIGUSR1);
                printf("envoyé au proc initial: %d\n",pids[0]);
                sleep(1);
                exit(0);
            }
        }
        
        /* Pere */
        else if( pid > 0 )
        {
            if(compteur == 0)
            {
                sigemptyset(&sig_proc);
                sigaction(SIGUSR1, &sig_proc,0);
                /* masquer SIGUSR1 */
                
                sigaddset (&sig_proc, SIGUSR1);
                sigprocmask (SIG_SETMASK,&sig_proc, NULL);
                
                /* attendre le signal SIGUSR1 */
                sigfillset (&sig_proc);
                sigdelset (&sig_proc, SIGUSR1);
                sigsuspend (&sig_proc);
                
                printf("message du dernier fils recu, envoi de SIGSTOP aux fils");
                for(i=0;i<nb_fils;i++){
                    kill(getppid(), SIGSTOP);
                }
                
                printf("Tous les descendants stoppés");
                
                for(i=0;i<nb_fils;i++){
                    kill(getppid(), SIGCONT);
                }
                
                
                sigemptyset(&sig_proc2);
                sigaction(SIGCHLD, &sig_proc2,0);
                /* masquer SIGCHLD */
                
                sigaddset (&sig_proc2, SIGCHLD);
                sigprocmask (SIG_SETMASK,&sig_proc2, NULL);
                
                /* attendre le signal SIGCHLD */
                sigfillset (&sig_proc2);
                sigdelset (&sig_proc2, SIGCHLD);
                sigsuspend (&sig_proc2);
                
                
                printf("Fin du programme");
                
            }
            else{
                sigemptyset(&sig_proc2);
                sigaction(SIGCHLD, &sig_proc2,0);
                /* masquer SIGCHLD */
                
                sigaddset (&sig_proc2, SIGCHLD);
                sigprocmask (SIG_SETMASK,&sig_proc2, NULL);
                
                /* attendre le signal SIGCHLD */
                sigfillset (&sig_proc2);
                sigdelset (&sig_proc2, SIGCHLD);
                sigsuspend (&sig_proc2);
                
            }
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
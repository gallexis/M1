#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sig_hand(int sig){
    printf ("signal recu %d %d \n",sig, getpid ());
}

int main (int argc, char ** argv)

{
    pid_t fils1, principal;
    int i=1;
    int ret;
    struct sigaction action;
    sigset_t sig_proc = (sigset_t) NULL;
    
    if (argc > 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    action.sa_mask=sig_proc;
    action.sa_flags=0;
    action.sa_handler = sig_hand;
    sigaction(SIGUSR1, &action,NULL);
    sigaction(SIGUSR2, &action,NULL);
    
    /* masquer SIGUSR1 e SIGUSR2*/
    sigaddset (&sig_proc, SIGUSR1);
    sigaddset (&sig_proc, SIGUSR2);
    sigprocmask (SIG_SETMASK, &sig_proc, NULL);
    
    principal = getpid();
    
    while (i<= 2)
    {
        if ( (ret=fork ()) ) {
            if (i==1)
                fils1 = ret;
            i++;
        }
        else
        {
            ret = fork ();
            break ;
        }
    }
    
    /* affichage */
    if (getpid() != principal)
    {
        printf ("pid %d; pid pere: %d \n", getpid (),getppid ());
        
        if ((ret !=0) && (i==2) )
            printf ("pid fils1 :%d\n", fils1);
    }
    
    /* petit-fils */
    if (ret == 0)
    {
        kill (getppid (), SIGUSR1);
    }
    else
    {
        /*les deux fils attendent SIGUSR1 de leur fils*/
        if (getpid () != principal)
        {
            sigdelset (&sig_proc, SIGUSR1);
            sigsuspend (&sig_proc);
            
            /* fils2 - envoie SIGUSR2 au frère (fils1)*/
            if (i==2)
            {
                kill (fils1, SIGUSR2);
            }
            
            /* fils1: attend SIGUSR2 de son frère (fils2)*/
            else
            {
                sigdelset (&sig_proc, SIGUSR2);
                sigsuspend (&sig_proc);
                
                /* envoie SIGUSR2 au processus main */
                kill (principal, SIGUSR2);
            }	      
        }
        else
        {
            /* le processus main attend SIGUSR2 de son premier fils (fils1) */ 
            sigdelset (&sig_proc, SIGUSR2);
            sigsuspend (&sig_proc);
        }
    }
}
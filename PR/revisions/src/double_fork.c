#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char ** argv)
{
    int i=1;
    int j=0;
    int ret, nb_fils;
    
    pid_t fils1, principal;
    
    if (argc > 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    principal = getpid ();
    
    /*création des processus */
    while (i<= 2)
    {
        if ( ( ret=fork() ) )
        {
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
    if (getpid ()!= principal)
    {
        printf ("pid %d, pid pere: %d \n", getpid (),getppid ());
        
        /* fils 2*/
        if ((ret !=0) && (i==2) )
            printf ("fils1 :%d\n", fils1);
    }
    
    
    if (ret!=0)
    {
        /* processus a des fils */
        nb_fils=1;
        if (getpid ()== principal)
            nb_fils++;
        
        for (j=0;j<nb_fils;j++)   
            wait (NULL);
    }
}

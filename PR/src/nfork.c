#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int nfork( int nb_fils){
    int compteur, compteurFilsCrees;
    pid_t pid;
    
    compteurFilsCrees = 0;
    
    for(compteur=0;compteur<nb_fils;compteur++)
    {
        pid = fork();
        
        /* Fils */
        if(pid == 0)
        {
            return 0;
        }
        
        /* Pere */
        else if( pid > 0 )
        {
            compteurFilsCrees++;
        }
        
        else
        {
            perror("probleme fork");
            
            if(compteurFilsCrees > 0)
                return compteurFilsCrees;
            else
                return -1;
        }
    }
    return compteurFilsCrees;
}

int main (int arg, char* argv []) {
    int p;
    int i=1; int N = 3;
    do {
        p = nfork (i) ;
        if (p != 0 )
            printf ("%d \n",p);
    } while ((p ==0) && (++i<=N));
    printf ("FIN \n");    
    return EXIT_SUCCESS;
}
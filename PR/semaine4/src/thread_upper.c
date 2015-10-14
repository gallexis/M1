#define _XOPEN_SOURCE 700
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include <pthread.h>

#define N 5

void *upper(void* path)
{
    FILE *fp1;
    FILE *fp2;
    int c = 1;
    
    fp1= fopen ((char*)path, "r");
    fp2= fopen ((char*)path, "r+");
    if ((fp1== NULL) || (fp2== NULL)) {
        perror ("fopen");
	    pthread_exit ((void*)0);
    }

    while (c !=EOF) {
        c=fgetc(fp1);
        if (c!=EOF)
        fputc(toupper(c),fp2);
    }

    printf("p: %s\n",(char*)path);
    fclose (fp1);
    fclose (fp2);

    pthread_exit ((void*)0);
}


int main(int argc, char *argv[])
{
    int i;
    int *status;
    
    pthread_t *tid;
    
    tid = malloc((argc-1)*sizeof(pthread_t));

    if(argc == 1){
        printf("missing arg");
    }    
        
    for(i=0 ; i < argc-1 ; i++)
    {
        if (pthread_create (&tid[i], NULL, upper, (void*)argv[i+1]) != 0) {
            perror("pthread_create \n");
            exit (1);
        }
    }

    
    for (i=0; i < argc-1; i++) 
    {
        if (pthread_join(tid[i], (void**) &status) != 0) {
            printf ("pthread_join"); 
            exit (1);
        }
        else
            printf ("Thread %d fini. \n",i);
    }

    
    return 0;
}

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>

int main(int argc, char *argv[]){
    int fic, NBL;
    pid_t prems;
    int i;
    char lu;
    pid_t pid;
    
    if(argc!=3){
        fprintf(stderr,"Nb argument incorrect\n");
        exit(EXIT_FAILURE);
    }
    
    NBL=atoi(argv[2]);
    if ((fic = open(argv[1],O_RDONLY))==-1){
        fprintf(stderr,"%s n'existe pas !!\n",argv[1]);
        exit(EXIT_FAILURE);
    }
    
    prems=getpid();
    
    for(i=0;i<NBL;i++)
    {
        if(read(fic,&lu,sizeof(char))!=0)
        {
            if((pid=fork())==-1)
            {
                fprintf(stderr,"Erreur fork\n");
                exit(EXIT_FAILURE);
            }
            
            if(pid!=0)
            {
                wait(NULL);
                printf("%d de pere %d a lu : %c\n",getpid(),getppid(), lu);
                break;
            }
        }
        else
        {
            fprintf(stderr,"Erreur lecture\n");
            exit(EXIT_FAILURE);
        }
    }
    
    if(prems==getpid())
        close(fic);
}
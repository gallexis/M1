#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define SIZE_TAMPON 100

char tampon [SIZE_TAMPON];


int copy(char* pathF1,char* pathF2)
{
    int fd1, fd2;
    int n;
    
    struct stat stat_info;
    
    
    if ( stat(pathF1, &stat_info) == -1)
    {
        perror ("erreur stat");
        return EXIT_FAILURE;
    }
    
    if( (stat_info.st_mode & S_IFREG) != S_IFREG )
    {
        printf ("fichier non regulier\n");
        return EXIT_FAILURE;
    }
    
    if (access (pathF1, R_OK) == -1 )
    {
        printf ("pas droit de lecture\n");
        return EXIT_FAILURE;
    }
    
    /* 2em fichier*/
    if (access (pathF2, F_OK) == 0 )
    {
        printf ("Le fichier existe déjà\n");
        return EXIT_FAILURE;
    }
    
    if ((fd2 = open (pathF2, O_WRONLY|O_CREAT, 0600)) == -1)
    {
        if(errno == EACCES)
            printf("L'accès  demandé  au  fichier  est interdit");
        else
            perror("open");
        
        return EXIT_FAILURE;
    }
    
    if ((fd1 = open (pathF1, O_RDONLY, 0600)) == -1)
        return EXIT_FAILURE;
    
    
    
    while((n= read (fd1,(char*)tampon, SIZE_TAMPON)) > 0) {
        
        if (write (fd2,(char*)tampon, n) == -1) {
            perror ("write");
            return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}

int main (int argc, char* argv[]) {
    
    if(argc != 3)
    {
        printf("pb arguments\n");
        return EXIT_FAILURE;
    }
    
    return copy(argv[1],argv[2]);
    
}
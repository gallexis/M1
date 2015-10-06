#define _XOPEN_SOUCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>

extern int errno;

#define _POSIX_SOUCE 1
#define TAILLE_PATH 100
#define SIZE_TAMPON 100

char tampon [SIZE_TAMPON];
char buff_path [TAILLE_PATH];
DIR *pt_Dir;
struct dirent* dirEnt;
char *ret;
char tmp[100];

int readInFile(char *pathFile,char *expr)
{
    int fd;
    struct stat stat_info;
    
    if ( stat(pathFile, &stat_info) == -1)
    {
        perror ("erreur stat");
        return EXIT_FAILURE;
    }
    
    if ( (stat_info.st_mode & S_IFREG) != S_IFREG )
    {
        printf ("fichier non regulier\n");
        return EXIT_FAILURE;
    }
    
    if ((fd = open (pathFile, O_RDONLY, 0600)) == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }
    
    while(read (fd,(char*)tampon, SIZE_TAMPON) > 0)
    {
        ret = strstr(tampon, expr);
        if(ret != NULL )
        {
            printf("%s\n",pathFile);
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}

int main (int argc, char* argv []) {
    int nbOcc;
    nbOcc = 0;
    
    if (argc < 3 ) {
        printf("Erreur arg");
        return 0;
    }
    
    memcpy (buff_path,argv[2],strlen(argv[2]));
    
    if ( ( pt_Dir = opendir (buff_path) ) == NULL) {
        perror("Repertoire");
        return 0;
    }
    
    /* lire répertoire */
    while ((dirEnt= readdir (pt_Dir)) != NULL) {
        strcpy(tmp,buff_path);
        
        strcat(tmp,"/");
        strcat(tmp,dirEnt->d_name);
        
        nbOcc += readInFile(tmp,argv[1]);
        memset(tmp, '\0', sizeof(tmp));
    }
    
    if(nbOcc == 0)
        printf("Aucun fichier valide\n");
    
    closedir (pt_Dir);
    
    return 0;
}
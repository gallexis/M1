#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define NB_FILS 2
#define TAILLE_BUF 1

void lire_read(int fd) {
    char buf[TAILLE_BUF];
    while (read(fd, buf, TAILLE_BUF) > 0) {
        printf("%s(lu par PID = %d)\n", buf, getpid());
    }
}

void lire_fgetc(FILE *const f) {
    char c;
    while (1) {
        c = fgetc(f);
        
        /* EOF */
        if (feof(f)) {
            break;
        }
        
        printf("%c(lu par PID = %d)\n", c, getpid());
    }
}

void lecture(char const * const nom_fichier) {

    int fd;
    FILE *f;
    int i;
    pid_t pid;
    
    for (i = 0; i < NB_FILS; ++i) {

        if ((pid = fork()) == -1) {
            perror("Erreur fork.\n");
            exit(EXIT_FAILURE);
        }
        
        /* Fils */
        if (pid == 0) {
            
            /* Fils n° 0 */
            if(i == 0)
            {
                if ((fd = open(nom_fichier, O_RDONLY, 0600)) == -1) {
                    perror("Erreur de l'ouverture du fichier (open).\n");
                    exit(EXIT_FAILURE);
                }
                
                lire_read(fd);
            }
            
            /* Fils n° 1 */
            else if(i == 1)
            {
                if ((f = fopen (nom_fichier, "r")) == NULL) {
                    perror("Erreur de l'ouverture du fichier (fopen).\n");
                    exit(EXIT_FAILURE);
                }
                
                lire_fgetc(f);
            }
            
            exit(EXIT_SUCCESS);
        }

    }
    
    
    /* Attendre fils */
    while ( waitpid(-1, NULL, 0) > 0 ) {
        printf("Fils terminé\n ");
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage : ./lectures fichier\n");
        return EXIT_FAILURE;
    }
    
    struct stat sb;
    if (stat(argv[1], &sb) == -1) {
        perror("Erreur stat.\n");
        return EXIT_FAILURE;
    }
    
    /* On vérifie que c'est bien un fichier régulier */
    if ((sb.st_mode & S_IFMT) != S_IFREG) {
        perror("Le fichier n'est pas un fichier régulier.\n");
        return EXIT_FAILURE;
    }
    
    printf("Fonction lecture\n");
    lecture(argv[1]);
    
    
    return EXIT_SUCCESS;
}

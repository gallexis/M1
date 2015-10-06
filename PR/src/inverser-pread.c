#define _POSIX_SOURCE 1
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int reverse(char* pathF1)
{
      int fd1, fd2;
      int position;
      char c[1];
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

        if ((fd1 = open (pathF1, O_RDONLY, 0600)) == -1)
              return EXIT_FAILURE;

        if ((fd2 = open ("b", O_WRONLY|O_CREAT, 0600)) == -1)
              return EXIT_FAILURE;

       position =  stat_info.st_size;

       lseek(fd1,0, SEEK_END);

        while( position-- > 0) {

            pread(fd1, (char*)c, sizeof(char), position);

            if (write (fd2,(char*)c, 1) == -1) {
                       perror ("write");
                       return EXIT_FAILURE;
            }
        }

return EXIT_SUCCESS;
}

int main (int argc, char* argv[]) {

        if(argc != 2)
        {
                printf("pb arguments\n");
                return EXIT_FAILURE;
        }

        return reverse(argv[1]);
}

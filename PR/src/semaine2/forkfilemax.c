#define _POSIX_SOURCE 1

#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int main(void) {
    int fd1, fd2, fd3;
    if ((fd1 = open ("./fich1", O_RDWR| O_CREAT | O_TRUNC, 0600)) == -1) {
        return EXIT_FAILURE;
    }
    
    if (write (fd1, "abcde", strlen ("abcde")) == -1) {
        return EXIT_FAILURE;
    }
    
    if (fork() == 0) {
        if ((fd2 = open("./fich1", O_RDWR)) == -1) {
            return EXIT_FAILURE;
        }
        if (write (fd1, "123", strlen("123")) == -1) {
            return EXIT_FAILURE;
        }
        if (write (fd2, "45", strlen("45")) == -1) {
            return EXIT_FAILURE;
        }
        
        /* Envoi du signal au père*/
        kill(getppid(), SIGINT);
        close(fd2);
    }
    else {
        /* Mise en place de la gestion du signal*/
        sigset_t sig_proc;
        sigemptyset(&sig_proc);
        sigaddset (&sig_proc,SIGINT);
        sigprocmask (SIG_SETMASK, &sig_proc, NULL);

        
        /* On force le fils à s'executer en premier, on attend
           son signal */
        pause();
        
        fd3 = dup(fd1);
        if (lseek(fd3, 0, SEEK_SET) == -1) {
            return EXIT_FAILURE;
        }
        if (write(fd3, "fg", strlen ("fg")) == -1) {
            return EXIT_FAILURE;
        }
        if (write(fd1, "hi", strlen ("hi")) == -1) {
            return EXIT_FAILURE;
        }
        
        wait(NULL);
        close(fd1);
        close(fd3);
        
        return EXIT_SUCCESS;
    }
}
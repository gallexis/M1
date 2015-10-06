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

#define SIZE_TAMPON 100
#define NB_FILS 4

char tampon [SIZE_TAMPON];

void remonte(char* file)
{
    int i,num;
    int randNum;
    pid_t pids[NB_FILS];
    FILE * fp;
    int fdf;
    char randNumChar;
    
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    
    for(i=0;i<NB_FILS;i++)
    {
        pids[i] = fork();
        
        /* Fils */
        if(pids[i] == 0)
        {
            srand(time(NULL));
            randNum = (int) (10*(float)rand()/ RAND_MAX);
            randNumChar = randNum + '0';
            
            printf("Fils créé\n ");
            
            if ((fdf = open(file, O_APPEND|O_WRONLY, 0600)) == -1)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            
            if (write (fdf,(char*) &randNumChar, strlen(&randNumChar)) == -1) {
                perror ("write--");
                exit(EXIT_FAILURE);
            }
            
            if (write (fdf,(char*) "\n", strlen("\n")) == -1) {
                perror ("write--");
                exit(EXIT_FAILURE);
            }
            
            exit(0);
        }
        
        /* Pere */
        else if( pids[i] > 0 )
        {
            
        }
        
        else
        {
            perror("probleme fork");
        }
    }
    
    
    while ( waitpid(-1, NULL, 0) > 0 ) {
        printf("Fils terminé\n ");
    }
    
    fp = fopen(file, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    num = 0;
    while((read= getline(&line, &len, fp)) > 0) {
        num+= atoi(line);
    }
    
    printf("num = %d\n",num);
   
}

int main(void)
{
    int fd;
    char *nameFile = "toto.txt";
    
    if ((fd = open(nameFile, O_CREAT, 0600)) == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    remonte(nameFile);
    
    return 0;
}
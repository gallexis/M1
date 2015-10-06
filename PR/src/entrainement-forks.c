#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void creerFils(int nbFils){

    int i,status;
    pid_t pids[nbFils];
    pid_t wpid;

    for(i=0;i<nbFils;i++)
    {
        pids[i] = fork();

        if(pids[i] == 0)
        {
            printf("Fils: pid = %d -- pid pere %d\n",getpid(),getppid());
            sleep(i+1);
            exit(0);
        }

        else if (pids[i]>0)
        {
            printf("$$ Pere: pid = %d -- pid fils %d\n",getpid(),pids[i]);

        }
    }

    int cpt = nbFils;
    while (cpt > 0)
    {
        if((wpid = waitpid(-1, &status, WNOHANG)) > 0)
        {
            printf("pid n°: %d exited\n",wpid);
            cpt--;
        }
    }

}


int main(int argc, const char * argv[]) {

    int nbFils;

    printf("Entrez nb fils: ");
    scanf("%d",&nbFils);

    printf("nombre fils: %d\n",nbFils);

    creerFils(nbFils);

    return 0;
}

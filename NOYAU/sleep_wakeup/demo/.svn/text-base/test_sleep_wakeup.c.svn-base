#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <sched.h>
#include <synch.h>

int obstacle;

void Thread1(int *pid) {
	int i;
	printf("Thread %d S'ENDORT \n", getfid());
	tsleep(MAXPRIO, &obstacle);
	printf("Thread %d S'EST REVEILLEE \n", getfid());
  for (i=0;i<1E9;i++) 
    if (i%(long)1E8 == 0) 
      printf("Thread %d \n",getfid());
}

void Thread2(int *pid) {
  long i;

  for (i=0;i<1E9;i++) {
    if (i%(long)1E8 == 0) 
      printf("Thread %d \n",getfid());
    if (i%(long)5E8 == 0) 
		 twakeup(&obstacle);
  }
  
}


int main (int argc, char *argv[]) {

  // Creer le premier thread
  CreateProc((function_t)Thread1,(void *)NULL, 0);

  // Creer le second thread
  CreateProc((function_t)Thread2,(void *)NULL, 0);


  // Lancer l'ordonnanceur en mode  "verbeux"
  sched(1);	

  // Imprimer les statistiques
  PrintStat();

  return EXIT_SUCCESS;

}

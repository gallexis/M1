#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <sched.h>
#include <synch.h>
#include <callo.h>

void func1 (char*);
void func2 (char*);
void func3 (char*);
void func4 (char*);

#define   TICK   100000

int flag_fin = 0;

struct	itimerval callout_tout = { { 0, TICK}, { 0, TICK} };     
struct timeval begin, end;

void print_time (char* name_func, char* appel)
{ 
  gettimeofday (&end,NULL);
 printf ("%ld TICKS: %s appelée par %s \n", ((unsigned long)(end.tv_usec - begin.tv_usec)+1000000*(end.tv_sec - begin.tv_sec))/TICK, name_func, appel);
 
}


void func1 (char* argv) {
  print_time ("func1",argv ); 
  /* appeler func 4 */
  timeout (func4,"func1" ,10);
}

void func2 (char* arg) {
   print_time ("func2",arg); 
   timeout (func3,"func2", 3);
}

void func3 (char* arg) {
 print_time ("func3",arg);  
}

void func4 (char* arg) {
  print_time ("func4",arg);
  /*  flag_fin =1; */
}


void Thread1(int *pid){

 
     timeout (func1,"thread1",8);
    
     timeout (func2,"thread1",14);
     
     timeout (func3,"thread1",12);

     
     while (! flag_fin) ;  

     printf("############ FIN LONG %d\n\n", *pid );
}




int main (int argc, char *argv[]) {

  // Créer les processus long
    CreateProc((function_t)Thread1,(void *)NULL, 0);

 // Lancer l'ordonnanceur en mode  "verbeux"
  sched(0);	

  // Imprimer les statistiques
  PrintStat();

  return EXIT_SUCCESS;

 
}

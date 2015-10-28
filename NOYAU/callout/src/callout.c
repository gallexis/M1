#include <stdio.h> 
#include <stdlib.h>

#include <callo.h> 
#include <sched.h> 
#include <proc.h>
#include <var.h> 


/**
 * @var struct itimerval callout_tout
 * Variable d'initialisation du temporisateur
 */
struct itimerval callout_tout;

/**
 * @var struct timeval begin
 * Date de début d'exécution
 */
struct timeval begin;

/**
 * @var struct timeval end
 * Date de fin d'exécution
 */
struct timeval end;

/**
 * @var int nbtick
 * Nombre de ticks écoulés
 */
int nbtick = 0;
/**
 * @var int cid
 * Identifiant d'un callout
 */
int cid = 0;

/**
 * @var static unsigned restart_flag
 * Flag actif si appel à la fonction restart en cours
 */
static unsigned restart_flag;



/********************************************************************************/

/**
 * @fn void sim_int_clock(void)
 * Fonction appelée à chaque tick (= handler de SIGALRM)
 */
void sim_int_clock() {
  struct callo *p1;
  int courant;      /* processus en cours d'exécution */
  

   if(verbose) {
    printf("TICK\n");
    fflush(stdout); 
    /* affichage du vecteur de callout */
    /*   printTab(); */
   } 
  
  /* augmentation du nombre de ticks utilisés par le processus */
  courant = GetElecProc();
  Tproc[courant].p_tick++;
  
  /* test pour la commutation */
  if(!--nbtick) {
    nbtick=Quantum;
    commut(NULL);
  }
  
  /* decrementer la premiere entree */
  MASK_ALRM;
  if (v.ve_callout[0].c_func != 0) {
    p1 = &(v.ve_callout[0]);
    while(p1->c_time<=0 && p1->c_func!=0)
      p1++;
    p1->c_time--;
  }
  
  /* restart va-t-elle être appelée */
  if((v.ve_callout[0].c_func!=0) && (v.ve_callout[0].c_time<=0))
    restart_flag |= CALLOUT;
  
  UNMASK_ALRM;
  
  /* printf("*** MAJ ***\n"); */
  /*   printTab();  */
  
  /* s'il y a de CALLOUT à traiter */
  if(restart_flag & CALLOUT) {
    /*     printf("CALLOUT\n"); */
    fflush(stdout);
    /* s'il n'y a pas de CALLOUT en traitement */
    if(!(restart_flag & RESTART_EXECUTION)) {
      /*       printf("RESTART\n"); */
      fflush(stdout);
      restart_flag &= ~CALLOUT;
      restart_flag |= RESTART_EXECUTION;
      restart();
      restart_flag &= ~RESTART_EXECUTION;
    } else {
      /*       printf("NO RESTART\n"); */
      fflush(stdout);
    }
  } else 
    ;/*     printf("pas de restart\n"); */
}

/********************************************************************************/
/**
 * @fn void callout_initialize(int tval)
 * Fonction permettant l'initialisation du mécanisme de callout
 * @param tval : durée d'un tick en microseconde
 */
void callout_initialize(int tval) {
  struct sigaction sa;
  
  callout_tout.it_interval.tv_sec  = 0;
  callout_tout.it_interval.tv_usec = tval;
  callout_tout.it_value.tv_sec  = 0;
  callout_tout.it_value.tv_usec = tval;

  sa.sa_handler = sim_int_clock;

  nbtick = Quantum;
  
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_NODEFER;
  
  sigemptyset(&mask);
  sigaddset(&mask, SIGALRM);

  sigaction(SIGALRM, &sa, NULL);
  
  setitimer(ITIMER_REAL, &callout_tout, NULL);
  gettimeofday(&begin, NULL);
}

/********************************************************************************/


/********************************************************************************/
/**
 * @fn extern void restart(void)
 * Exécute les fonctions figurant dans le vecteur de callout dont le compteur est inférieur ou égal à zéro
 */
void restart(void) {
  /* TME A COMPLETER */  

}

/********************************************************************************/
/**
 * @fn int timeout(void (*fun)(), void *arg, int tim)
 * Crée une nouvelle entrée dans le vecteur de callout et l'initialise avec les paramétres suivants :
 *  - fun
 *  - arg
 *  - tim
 * @param fun : fonction à appeler
 * @param arg : argument de la fonction fun
 * @param tim : durée en nombre de ticks aprés laquelle fun est appelée
 * @return L'identifiant du callout créé
 */
int timeout(void (*fun)(), void *arg, int tim){ 
  
  /* TME - A COMPLETER */
  return 0;
}

/********************************************************************************/
/**
 * @fn void untimeout(int ident)
 * Permet la suppression d'un callout
 * @param ident : identifiant du callout à supprimer
 */
void untimeout(int ident) {
  
  /* TME - A COMPLETER */  

  } 

/********************************************************************************/
/**
 * @fn void printTab(void)
 * Affichage du vecteur de callout
 */
void printTab(void) {
  
  struct callo *p1;
  int entree = 0;
  p1 = &v.ve_callout[0];
  
  if(p1->c_func==NULL) {
    printf("Table vide\n");
    fflush(stdout);
    return;
  }
  
  while(p1->c_func) {
    printf("> Entree %d : \n", entree);
    fflush(stdout);
    printf("> ------ c_time : %d\n", p1->c_time);
    fflush(stdout);
    printf("> ------ c_arg  : %p\n", p1->c_arg);
    fflush(stdout);
    printf("> ------ c_func : %p\n", p1->c_func);
    fflush(stdout);
    printf("> ------ c_cid   : %d\n", p1->c_cid);
    fflush(stdout);
    p1++;
    entree++;
  }
}

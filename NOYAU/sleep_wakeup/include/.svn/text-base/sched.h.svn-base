/**
 * @file sched.h
 * @author Karagiannis Antonios
 *
 * Mini-ordonnanceur de fonctions utilisateurs 
 */

#ifndef _SCHED_H
#define _SCHED_H


#include <sys/time.h>
#include <signal.h>


/**
 * @def main
 * Définition de la constante main
 */
#define main  MAIN


/* Parametres de l'ordonnanceur */
/**
 * @def TICK
 * Constante définissant la durée d'un tick en microseconde par défaut
 */
#define TICK          100000
/**
 * @def TICKS_PAR_SEC
 * Constante définissant le nombre de ticks par seconde par défaut
 */
#define TICKS_PAR_SEC 10
/**
 * @def QUANTUM
 * Constante définissant la durée d'un quantum en nombre de ticks
 */
#define QUANTUM       10


/* Types d'ordonnancement */
/**
 * @def NEW
 * Nouvelle strategie definie par l'utilisateur 
 */
#define NEW       1
/**
 * @def PREMPT
 * Ordonnancement preemptif 
 */
#define PREMPT    2

/**
 * @def BATCH
 * Ordonnancement de type Batch 
 */
#define BATCH     3    


/**
 * @def MAXPROC
 * Nombre maximum de processus 
 */
#define MAXPROC   15   


/* les differentes priorites */
/**
 * @def MINPRIO
 * Priorité minimum
 */
#define MINPRIO     0
/**
 * @def MAXPRIO
 * Priorité maximum
 */
#define MAXPRIO     100
/**
 * @def MAXUSERPRIO
 * Priorité maximum pour l'utilisateur
 */
#define MAXUSERPRIO 50

/* Etat d'un processus */
/**
 * @def RUN
 * État RUN
 */
#define RUN   1
/**
 * @def IDLE
 * État IDLE
 */
#define IDLE  2
/**
 * @def ZOMB
 * État ZOMB
 */
#define ZOMB  3
/**
 * @def SLEEP
 * État SLEEP
 */
#define SLEEP 4


/**
 * @var typedef void (*function_t)()
 * Type des fonctions exécutées par les processus de l'ordonnanceur
 */
typedef void (*function_t)();

/**
 * @var extern int Quantum
 * Valeur du quantum en nombre de ticks
 */
extern int Quantum;          
/**
 * @var extern int Tick
 * Valeur du tick en micro secondes 
 */
extern int Tick;
/** 
 * @var extern int Ticks_par_sec
 * Nombre de ticks par secondes
 */
extern int Ticks_par_sec;    
/**
 * @var extern int nbrun
 * Nombre de processus prets 
 */
extern int nbrun;            
/**
 * @var extern int nbsleep
 * Nombre de processus sleep 
 */
extern int nbsleep;         

/**
 * @var extern int verbose
 * Mode verbeux
 */
extern int verbose;

/* Macros pour bloquer l'horloge */
/**
 * @var sigset_t mask
 * Masque de signaux
 */
sigset_t mask;
/**
 * @def MASK_ALRM
 * Masque le signal SIGALRM
 */
#define MASK_ALRM    sigprocmask(SIG_BLOCK, &mask, NULL)
/**
 * @def UNMASK_ALRM
 * Démasque le signal SIGALRM
 */
#define UNMASK_ALRM  sigprocmask(SIG_UNBLOCK, &mask, NULL)


/* fonctions de l'ordonnanceur */
/**
 * @fn void sched(int print)
 * Boucle de l'ordonnanceur 
 * @param print : mode verbeux
 */
void sched(int print);

/**
 * @fn int CreateProc(function_t f, void *arg, int d)
 * Créer un nouveau processus
 * @param f : fonction exécutée par le processus
 * @param arg : argument de la fonction f
 * @param d : estimation de la durée du processus
 * @return rend 0
*/
int  CreateProc(function_t, void *, int);

/**
 * @fn void InitSched(void)
 * Initialisation de la table des processus et création du processus IDLE
 */
void InitSched(void);                    

/**
 * @fn void SchedParam(int type, int timeslice, int tick, int (*elec)(void))
 * Fixer les paramètres de l'ordonnanceur
 * @param type : type de l'ordonnacement
 * @param timeslice : durée d'un quantum en nombre de ticks
 * @param tick : durée d'un tick
 * @param elec : pointeur vers la fonction d'élection
 */
void SchedParam(int, int, 
		int, int (*f)(void));    

/**
 * @fn void PrintStat(void)
 * Affiche les statistiques de toutes les taches 
 */
void PrintStat(void);

/** 
 * @fn int GetElecProc(void)
 * Fonction qui retourne le processus élu
 * @return numéro de l'entrée dans la table des processus de l'élu
 * @return -1 si il n'y en a aucun 
 */
int  GetElecProc(void);

/**
 * @fn void commut(char* arg)
 * Fonction appelée toutes les "Quantum" ticks pour faire la commutation 
 * @param arg est toujours égal à NULL, il a été ajouté pour pouvoir effectuer des appels à cette fonction via un timeout
 */
void commut(char*);      

/**
 * @fn int getfid(void)
 * Retourne l'identifiant du processus courant
 * @return pid
 * @return -1 en cas d'erreur
 */
int  getfid(void);                          

#endif /* _SCHED_H */

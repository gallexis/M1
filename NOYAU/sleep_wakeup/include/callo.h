/**
 * @file callo.h
 * @author Karagiannis Antonios
 */
#ifndef _CALLO_H
#define _CALLO_H

#include <stdlib.h>
#include <types.h>

/**
 * @def CALLOUT
 * Constante représentant un callout
 */
#define CALLOUT            1
/**
 * @def RESTART_EXECUTION
 * Constante représentant un appel à la fonction restart
 */
#define RESTART_EXECUTION  2

/**
 * @struct callo
 * Définition de la structure callo
 */
struct callo {
  int        c_time;     /**< incremental time */
  bc_caddr_t c_arg;      /**< argument to routine */
  void       (*c_func)();  /**< routine */
  int         c_cid;      /**< callout ID */
};

/**
 * @var extern int cid
 * Identifiant d'un callout
 */
extern int cid;   
/**
 * @var extern int nbtick
 * Nombre de ticks écoulés
 */
extern int nbtick;

/**
 * @fn void callout_initialize(int tval)
 * Fonction permettant l'initialisation du mécanisme de callout
 * @param tval : durée d'un tick en microseconde
 */
extern void callout_initialize(int);

/**
 * @fn void print_time(char* name_func, char* appel)
 * Fonction d'affichage
 * @param name_func : nom de fonction
 * @param appel : nom de la fonction ayant appelé name_func
 */
extern void print_time(char*, char*);

/**
 * @fn extern void restart(void)
 * Exécute les fonctions figurant dans le vecteur de callout dont le compteur est inférieur ou égal à zéro
 */
extern void restart(void);

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
extern int timeout(void(*)(), void*, int);

/**
 * @fn void untimeout(int ident)
 * Permet la suppression d'un callout
 * @param ident : identifiant du callout à supprimer
 */
extern void untimeout(int);

/**
 * @fn void printTab(void)
 * Affichage du vecteur de callout
 */
extern void printTab(void);

#endif /* _CALLO_H */

/**
 * @file proc.h
 * @author Karagiannis Antonios
 */
#ifndef _PROC_H
#define _PROC_H

#include <setjmp.h>

/**
 * @struct proc
 * Définition de la structure proc
 */
struct proc {
  int p_flag;                       /**< Etat de la tache */
  int p_pri;                        /**< Priorité */
  int p_usrpri;                     /**< Priorité en mode utilisateur */
  int p_pid;                        /**< Pid */
  sigjmp_buf p_env;                 /**< Contexte matériel (registres) */
  function_t p_func;                /**< Le code */
  void *p_arg;                      /**< Les arguments */
  unsigned long p_stack_size;       /**< La taille de la pile d'exécution */
  char *p_stack_svg;                /**< La pile */
  struct timeval p_end_time;        /**< date de fin */
  struct timeval p_start_time;      /**< date de création */
  struct timeval p_realstart_time;  /**< date de lancement */
  double p_ncpu;                    /**< "cpu" consommés */
  double p_tick;                    /**< nombre de ticks consommés */
  double p_duration;                /**< temps estimé de la tâche */
  void *p_ptr;                      /**< Pointeur pour données additionnelles */
};

/**
 * @var struct proc Tproc[MAXPROC+1]
 * Table des descripteurs de processus
 */
struct proc Tproc[MAXPROC+1];

#endif /* _PROC_H */

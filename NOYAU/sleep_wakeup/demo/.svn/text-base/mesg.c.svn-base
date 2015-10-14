/*
 *
 * Implémentation d'un mecanisme d'echange d'information
 * entre fonction suivant un schéma Producteur / Consommateur
 * (Pierre.Sens@lip6.fr)
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <sched.h>
#include <synch.h>

#include "mesg.h"


#define NBMSGPROD 50  /* Nombre de messages produits par producteur */
#define NBPROD 2 		 /* Nombre de producteur */

int vlock =0;

void lock() {
	MASK_ALRM;
	while (vlock == 1) {
		tsleep(MAXUSERPRIO + 3, &vlock);
	}
	vlock = 1;
	UNMASK_ALRM;
}

void unlock() {
	vlock = 0;
	twakeup(&vlock);
}


t_fmsg *CreerFile(void) {
	t_fmsg *f;
	if ((f = (t_fmsg *) malloc(sizeof(t_fmsg))) == NULL) {
		perror("Malloc");
		exit(1);
	}
	f->deposer = 0;
	f->retirer = 0;
	f->nb_msg = 0;
	return f;
}


/*
 * Ajouter le message dont le contenu est désigné par d
 * dans la file f
 */

void DeposerFile(t_fmsg *f, void *d) {

	/* Code du TP à ajouter */

}	

/*
 * Retirer de la file f un messsage (dans l'ordre FIFO)
 * En sortie exp contient un pointeur sur l'expediteur
 * Cette fonction retourne un pointeur sur le message extrait
 */

void *RetirerFile(t_fmsg *f, int *exp) {

	/* Code du TP à ajouter */
	
}
				
void DetruireFile(t_fmsg *f) {
	free(f);
}


/* 
 * Exemple de producteur
 */

void Producteur(t_fmsg *f) {
	int i;

	printf("Producteur numéro %d \n", getfid());

	
	for (i = 0; i < NBMSGPROD; i++) {
		char *msg;
		if ((msg = malloc(80)) == NULL) {
			perror("Prod - malloc");
			exit(1);
		}
		sprintf(msg, "MESSAGE Numero %d", i);
		DeposerFile(f, (void *)msg);
		printf("Production %d de %d\n", i, getfid());
	}
}

/* 
 * Exemple de consommateur
 */

void Consommateur(t_fmsg *f) {
	int i, exp;

	printf("Consommateur numéro %d \n", getfid());

	
	for (i = 0; i < NBMSGPROD*NBPROD; i++) {
		char *msg;
		msg = (char *) RetirerFile(f, &exp);
		printf("From : %d, %s\n", exp, msg);
		free(msg);
	}
}


int main (int argc, char *argv[]) {
	t_fmsg *f;
	int i;

	
	/* Creer la file */
	f = CreerFile();
	
  // Créer le thread consommateur
  CreateProc((function_t)Consommateur,(void *)f, 0);

  // Créer les threads producteurs
  for (i = 0; i < NBPROD; i++) 
	  CreateProc((function_t)Producteur,(void *)f, 0);



  // Lancer l'ordonnanceur en mode non "verbeux"
  sched(1);	

  // Imprimer les statistiques
  PrintStat();

  return EXIT_SUCCESS;

}

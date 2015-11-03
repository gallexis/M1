#define _XOPEN_SOURCE 700
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include <pthread.h>

#include "../include/producteur_consommateur.h"

#define N 100

pthread_mutex_t p = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t fullStack = PTHREAD_COND_INITIALIZER;
pthread_cond_t emptyStack = PTHREAD_COND_INITIALIZER;

int stack[N];
int pointeur;

void push(int c)
{
    pthread_mutex_lock(&p);
    
    while( pointeur == (N-1) ) {
        pthread_cond_wait(&fullStack,&p);
    }
    stack[pointeur] = c;
    
    if ( pointeur == 0 )
    {
        pthread_cond_broadcast(&emptyStack);
    }
    
    pointeur++;
    
    pthread_mutex_unlock (&p);
}


int pop()
{
    int c;
    pthread_mutex_lock(&p);
    
    while ( pointeur == 0 ) {
        pthread_cond_wait(&emptyStack,&p);
    }
    pointeur--;
    
    c =  stack[pointeur];
    
    if( pointeur == (N-1) ) {
        pthread_cond_broadcast(&fullStack);
    }
    
    
    pthread_mutex_unlock (&p);
    
    return c;
}

void *producteur()
{
    
    PRODUCTEUR
    
    pthread_exit ((void*)0);
}


void *consommateur()
{
    
    CONSOMMATEUR
    
    
    pthread_exit ((void*)0);
}


int main(int argc, char *argv[])
{
    int *status;
    pthread_t *prods;
    pthread_t *cons;
    int maxProds;
    int maxCons;
    int i;
    
    maxProds = atoi(argv[1]);
    maxCons = atoi(argv[2]);
    
    pointeur = 0;
    
    prods = malloc(maxProds*sizeof(pthread_t));
    cons = malloc(maxCons*sizeof(pthread_t));
    
    if(argc < 2){
        printf("erreur arg");
        return 0;
    }
    
    /* -- CREATE -- */
    for(i=0 ; i < maxProds ; i++)
    {
        if (pthread_create (&prods[i], NULL, consommateur, (void*)0) != 0) {
            perror("pthread_create \n");
            exit (1);
        }
    }
    
    for(i=0 ; i < maxCons ; i++)
    {
        if (pthread_create (&cons[i], NULL, producteur, (void*)0) != 0) {
            perror("pthread_create \n");
            exit (1);
        }
    }
    
    
    /* -- WAIT -- */
    for(i=0 ; i < maxProds ; i++)
    {
        if (pthread_join(prods[i], (void**) &status) != 0) {
            printf ("pthread_join");
            exit (1);
        }
        else
            printf ("Thread %d fini. \n",i);
    }
    
    for(i=0 ; i < maxCons ; i++)
    {
        if (pthread_join(cons[i], (void**) &status) != 0) {
            printf ("pthread_join");
            exit (1);
        }
        else
            printf ("Thread %d fini. \n",i);
    }
    
    
    return 0;
}

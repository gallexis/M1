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
    
        if( pointeur == (N-1) ) {
            pthread_cond_wait(&fullStack,&p);
        }    
        stack[pointeur] = c;
        
        if( pointeur == 0 )
        {
            pthread_cond_signal(&emptyStack);
        }
    
    pointeur++;
    
    pthread_mutex_unlock (&p); 
}


int pop()
{
    int c;
    pthread_mutex_lock(&p);
    
        if ( pointeur == 0 ) {
            pthread_cond_wait(&emptyStack,&p);
        } 
        pointeur--;
    
        c =  stack[pointeur];
    
        if( pointeur == (N-1) ) {
            pthread_cond_signal(&fullStack);
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


int main()
{
    int *status;
    pthread_t t1, t2;
    pointeur = 0;
    
                            /* -- CREATE -- */
    
    /* producteur */
    if (pthread_create (&t2, NULL, producteur, (void*)0) != 0) {
        perror("pthread_create \n");
        exit (1);
    }

    /* consommateur */
    if (pthread_create (&t1, NULL, consommateur, (void*)0) != 0) {
        perror("pthread_create \n");
        exit (1);
    }  
    
                            /* -- WAIT -- */
    /* consommateur */
    if (pthread_join(t1, (void**) &status) != 0) {
        printf ("pthread_join"); 
        exit (1);
    }

    /* producteur */  
    if (pthread_join(t2, (void**) &status) != 0) {
        printf ("pthread_join"); 
        exit (1);
    }

    
    return 0;
}

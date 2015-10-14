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
            pthread_cond_broadcast(&emptyStack);
    
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
          
        c =  stack[pointeur];
        pointeur--;
        
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


int main()
{
    int *status;
    pthread_t id1;
    pthread_t id2;
    pointeur = 0;   
    id1 = 1;
    id2 = 2;       
        
                            /* -- CREATE -- */
    /* consommateur */
    if (pthread_create (&id1, NULL, consommateur, (void*)0) != 0) {
        perror("pthread_create \n");
        exit (1);
    }  
    
    /* producteur */   
    if (pthread_create (&id2, NULL, producteur, (void*)0) != 0) {
        perror("pthread_create \n");
        exit (1);
    }      
    
                            /* -- WAIT -- */
    /* consommateur */
    if (pthread_join(id1, (void**) &status) != 0) {
        printf ("pthread_join"); 
        exit (1);
    }

    /* producteur */  
    if (pthread_join(id2, (void**) &status) != 0) {
        printf ("pthread_join"); 
        exit (1);
    }

    
    return 0;
}

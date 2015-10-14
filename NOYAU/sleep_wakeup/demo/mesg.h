/*
 * Structure d'une file de message 
 *
 * (Pierre.Sens@lip6.fr)
 */ 

/* Un message */

typedef struct t_msg {
        int exp;
        void *data;
} t_msg;

/* Une file de message */

#define MAXMSG 8        /* Nombre maximum de messages dans une file */
typedef struct t_fmsg {
        t_msg file[MAXMSG]; /* Les messages */
        int nb_msg;                     /* Nombre de messages */
        unsigned int deposer; /* indice pour déposer */
        unsigned int retirer; /* indice pour retirer */
        void *file_pleine;      /* Condition d'attente file pleine */
        void *file_vide;                /* Condition d'attente file vide */
} t_fmsg;

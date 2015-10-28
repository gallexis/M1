#ifndef _SYNCH_H
#define _SYNCH_H

/**
 * @fn int tsleep(int pri, void *obs)
 * Permet au processus de s'endormir sur une ressource
 * @param pri : priorité au reveil
 * @param obs : ressource observée
 * @return rend 0
 */
int tsleep(int, void*);

/**
 * @fn int twakeup(void *obs)
 * Permet au processus de reveiller l'ensemble des processus endormis sur une certaine ressource 
 * @param obs : ressource observée
 * @return rend -1 en cas d'erreur sinon 0
 */
int twakeup(void*);

/**
 * @fn int tsleep_time(int sec)
 * Permet au processus de s'endormir pendant une durée exprimée en seconde
 * @param sec : durée de l'endormissement
 * @return rend -1 en cas d'erreur sinon 0
 */
int tsleep_time(int sec); 

#endif /* _SYNCH_H */

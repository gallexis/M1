/**
 * @file conf.h
 * @author Karagiannis Antonios
 */
#ifndef _CONF_H
#define _CONF_H

/** 
 * @struct bdevsw
 * Declaration of block device
 * switch. Each entry (row) is
 * the only link between the
 * main unix code and the driver.
 * The initialization of the
 * device switches is in the
 * file conf.c.
 */
struct bdevsw {
  int	     (*d_strategy)();
} *bdevsw;

/**
 * @var nblkdev
 * Nombre de devices
 */
int nblkdev;


int adddev(int (*)());
void pbdevsw(void);

void init_poolbuf(int);
void pbuf(void);

void init_hbuf(int, long);
void phbuf(void);

void printfreelist(void);

#endif /* _CONF_H */

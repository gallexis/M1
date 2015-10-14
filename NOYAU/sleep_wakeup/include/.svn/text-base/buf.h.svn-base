/**
 * @file buf.h
 * @author Karagiannis Antonios
 */
#ifndef _BUF_H
#define _BUF_H

/**
 * Taille maximum
 */
#define BSIZE 1024

#include <types.h>


/**
 * @struc buf
 * @brief Structure représentant un buffer cache
 *
 *
 * Each buffer in the pool is usually doubly linked into 2 lists:
 * the device with which it is currently associated (always)
 * and also on a list of blocks available for allocation
 * for other use (usually).
 * The latter list is kept in last-used order, and the two
 * lists are doubly linked to make it easy to remove
 * a buffer from one list when it was found by
 * looking through the other.
 * A buffer is on the available list, and is liable
 * to be reassigned to another disk block, if and only
 * if it is not marked BUSY.  When a buffer is busy, the
 * available-list pointers can be used for other purposes.
 * Most drivers use the forward ptr as a link in their I/O
 * active queue.
 * A buffer header contains all the information required
 * to perform I/O.
 * Most of the routines which manipulate these things
 * are in bio.c.
 *
 */
struct buf {
  int         b_flags;      /**< buffer flags */
  struct buf *b_forw;       /**< previous buf on b_list */
  struct buf *b_back;       /**< next buf on b_list */
  struct buf *av_forw;      /**< previous buf on av_list */
  struct buf *av_back;      /**< next buf on av_list */
  int         b_dev;        /**< major+minor device name */
  int         b_count;      /**< transfer count */
  union {
    bc_caddr_t  *b_un_addr; /**< low order core address */
  } b_un;
  int        *b_xmem;       /**< transfer memory address */
  int         b_base;       /**< page number for physical i/o */
  int         b_size;       /**< number of pages physical i/o */
  bc_daddr_t  b_blkno;      /**< block number on device */
  char        b_error;      /**< returned after I/O */
  char       *b_resid;      /**< bytes not transferred */
  int         b_pri;        /**< priority */
};

/**
 * Macro permettant l'accés au core du buffer
 */
#define b_addr b_un.b_un_addr

/*
 * These flags are kept in b_flags
 */
#define	B_WRITE	 0x0000  /**< non-read pseudo-flag */
#define	B_READ	 0x0001  /**< read when I/O occurs */
#define	B_DONE	 0x0002  /**< transaction finished */
#define	B_ERROR	 0x0004  /**< transaction aborted */
#define	B_BUSY	 0x0010  /**< not on av_forw/back list */
#define	B_PHYS   0x0020  /**< wait I/O completion : physical I/O */
#define B_WANTED 0x0100  /**< issue wakeup when BUSY goes off */ 
#define B_AGE    0x0200  /**< delayed write for correct aging */
#define	B_ASYNC	 0x0400  /**< don't wait for I/O completion */
#define	B_DELWRI 0x1000  /**< don't write till block leaves available list */


/**
 * Nombre de buffers
 */
int nbbuf;

struct buf *buf;        /**< The buffer pool itself */
struct buf  bfreelist;  /**< head of available list */
/* extern char buffers[][BSIZE]; */

/**
 * Fast access to buffers in cache by hashing
 */
#define bhash(d,b)     ((struct buf *)&hbuf[((int)d+(int)b)&v.v_hmask])

/**
 * @struct hbuf
 * Table de hachage contenant les buffers en cours d'utilisation
 */
struct hbuf {
  int         b_flags;   /**< Ce champs ne représente rien, 
			    il rend le cast d'un variable de type
			    hbuf en type buf (les deux structures 
			    sont composées des mêmes champs */
  struct buf *b_forw;   /**< Pointeur vers le buffer suivant */
  struct buf *b_back;   /**< Pointeur vers le buffer précédent */
};

/**
 * Variable représentant la table de hachage des buffers
 */
struct hbuf *hbuf;

/**
 * @fn void iodone(struct buf *bp)
 * Permet de signaler la fin d'une E/S sur un buffer et reveille les processus endormis
 * sur le buffer (à condition qu'il en existe).
 *
 * Mark I/O complete on a buffer, release it if I/O is asynchronous,
 * and wake up anyone waiting for it.
 * @param bp : pointeur vers un buffer
 */
extern void iodone(struct buf *bp);

/**
 * @fn struct buf *bread(int dev, bc_daddr_t blkno)
 * Permet la lecture d'un bloc physique (si nécésaire) et rend 
 * le buffer lu
 *
 * Read in (if necessary) the block and return a buffer pointer
 * @param dev : device (major+minor)
 * @param blkno : numéro de bloc
 * @return le buffer associé au device dev et de numéro de bloc blkno
 */
struct buf *bread(int dev, bc_daddr_t blkno);

/**
 * @fn struct buf *breada(int dev, bc_daddr_t blkno, bc_daddr_t rablkno)
 * Lit le bloc demandé et lance une e/s sur le bloc suivant du device
 *
 * Read in the block, like bread, but also start I/O on the
 * read-ahead block (which is not allocated to the caller)
 * @param dev : device (major+minor)
 * @param blkno : numéro de bloc
 * @param rablkno : read-ahead bloc
 * @return le buffer associé au device dev et de numéro de bloc blkno
 */
struct buf *breada(int dev, bc_daddr_t blkno, bc_daddr_t rablkno);

/**
 * @fn void bwrite(struct buf *bp)
 * Permet l'ecrire sur disque du buffer et la libération de celui-ci si l'ecriture est synchrone
 *
 * Write the buffer, waiting for completion.
 * Then release the buffer.
 * @param bp : pointeur vers un buffer
 */
extern void bwrite(struct buf *bp);

/**
 * @fn void bdwrite(struct buf *bp)
 * Marque le buffer à sale et le libére
 * 
 * Release the buffer, marking it so that if it is grabbed
 * for another purpose it will be written out before being
 * given up (e.g. when writing a partial block where it is
 * assumed that another write for the same block will soon follow).
 * This can't be done for magtape, since writes must be done
 * in the same order as requested.
 * @param bp : pointeur vers un buffer
 */
extern void bdwrite(struct buf *bp) ;

/**
 * @fn void bawrite(struct buf *bp)
 * Libére le buffer et effectue l'écriture en asynchrone
 *
 * Release the buffer, start I/O on it, but don't wait for completion.
 * @param bp : pointeur vers un buffer
 */
extern void bawrite(struct buf *bp);

/**
 * @fn void brelse(struct buf *bp)
 * Libere le buffer sans générer de E/S.
 *
 * Release the buffer, with no I/O implied.
 * @param bp : pointeur vers un buffer
 */
extern void brelse(struct buf *bp);

/**
 * @fn struct buf *getblk(int dev, bc_daddr_t blkno)
 * Permet de récuperer un buffer en fonction d'un device et d'un numéro de bloc.
 * S'il n'existe pas dans la table de hachage des buffers précédemment alloués, un buffer est
 * retiré de la liste des buffers disponibles et alloué au device et au bloc correspondant
 * 
 * Assign a buffer for the given block. If the appropriate
 * block is already associated, return it; otherwise search
 * for the oldest non-busy buffer and reassign it.
 * @param dev : device (major+minor)
 * @param blkno : numéro de bloc
 * @return le buffer associé au device dev et de numéro de bloc blkno
 */
extern struct buf *getblk(int dev, bc_daddr_t blkno);

#endif /* _BUF_H */

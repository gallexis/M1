#define _SVID_SOURCE 1
#define _REENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>


struct sembuf op;
key_t clef;
int semid ,i;

void wait_barrier (N){

  semop(semid, &op, 1);

  op.sem_op = 0;

  semop(semid, &op, 1);

}

void process (int N) {
  printf ("proc= %d avant barrière\n",getpid());
 wait_barrier (N);
  printf ("proc= %d après barrière\n",getpid());
  exit (0);
}


int main(int argc, char **argv)
{
int N =  atoi (argv[1]);

clef = ftok(argv[0], 0);
semid = semget(clef, 1,IPC_CREAT | IPC_EXCL | 0666);
semctl(semid, 0, SETVAL, N);
int sem_value = semctl(semid, i, GETVAL, 0);
printf("sem value = %d\n",sem_value);
op.sem_num = 0;
op.sem_flg = 0;
op.sem_op = -1;

i=1;
pid_t pid;
for( i=0;i<N;i++){
  if ((pid = fork ()) == 0 ){

    process(N);
  }
}

for( i=0;i<N;i++)
  wait(NULL);


semctl(semid, 0, IPC_RMID, 0);

return 0;
}

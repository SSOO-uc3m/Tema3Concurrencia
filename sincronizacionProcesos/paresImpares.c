/* Este	programa crean	2 hijos	(procesos pesados), uno	escribe	los números pares (del 2 al 10)	y el otro
* los	impares	(del 1	al 9).
* En pantalla deben aparecer los números ordenados por	lo que	las ejecuciones	deben ser alternas
*/
//José	Manuel	Pérez Lobato
// Carlos Tessier

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/sem.h>
#include	<sys/wait.h>
#include	<unistd.h>

const int MAX = 10;

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
}; 


void	hijoImpares(int id_conj_sem){
	int i;

	struct	sembuf	restar	= {0,-1,0}; // decrementa el semáforo Impares
	struct	sembuf	sumar	= {1,1,0};  // aumenta el semáforo Pares

	for (i=1;i<MAX;i=i+2){
	 semop	(id_conj_sem, &restar, 1);
	 printf	("HijoImpares :%d\n",i);
	 semop	(id_conj_sem, &sumar,	1);
	}
}

void	hijoPares(int id_conj_sem){
	int	i;
	struct	sembuf	operacion;
	operacion.sem_flg=0;

	for (i=2;i<=MAX;i=i+2){
		operacion.sem_num=1;
	 	operacion.sem_op=-1;
	 	semop (id_conj_sem, &operacion, 1); /* decrementa el semáforo Pares*/
 	 	printf	("HijoPares :%d\n",i);
		operacion.sem_num=0;
		operacion.sem_op=1;
		semop (id_conj_sem, &operacion,	1); /* aumenta el semáforo impares*/
	}
}

int main(){
	int pid1,pid2;
	int id_conj_sem;
	union semun op_sem;

	if (( id_conj_sem = semget ( /* Abrimos o creamos un conjunto de semáforos */
		35, /* con una cierta clave */
		2, /* con dos elementos*/
		 IPC_CREAT|0666))==-1) /* lo crea (IPC_CREAT) con los permisos 0666*/
	exit(-1);

	op_sem.val=1;
	semctl	(id_conj_sem, 0, SETVAL, op_sem); // valor inicial 0 del semáforo pares
	op_sem.val=0;
	semctl	(id_conj_sem, 1, SETVAL, op_sem); // valor inicial 1 del semáforo impares

	if ((pid1=fork())==0)
		hijoPares(id_conj_sem);
	else if	((pid2=fork())==0)
		hijoImpares(id_conj_sem);

	wait(NULL);
	wait(NULL);
	semctl	(id_conj_sem, 1,IPC_RMID, op_sem); // liberamos semáforos
}


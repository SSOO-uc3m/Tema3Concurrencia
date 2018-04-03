#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

int dato = 5;       /* recurso */
int n_lectores = 0; /* numero de lectores */
sem_t sem_lec;      /* controlar el acceso n_lectores */
sem_t mutex;        /* controlar el acceso a dato */

void *lector(void *arg) { /* codigo del lector */
  int i;
  for(;;){

  	sem_wait(&sem_lec);
  	n_lectores = n_lectores + 1;
  	if (n_lectores == 1) sem_wait(&mutex);
  	sem_post(&sem_lec);

  	printf("\tL:lee dato %d (num lectores %d)\n",dato,n_lectores);  /* leer dato */

  	sem_wait(&sem_lec);
  	n_lectores = n_lectores - 1;
  	if (n_lectores == 0) sem_post(&mutex);
  	sem_post(&sem_lec);
 }
 //pthread_exit(0);
}

void *escritor(void *arg) {   /* codigo del escritor */
   int i;
   for(;;){
   	sem_wait(&mutex);
   	dato = dato + 2;     /* modificar el recurso */
   	printf("P: modifico dato por %d\n",dato);
   	sem_post(&mutex);
   }
   //pthread_exit(0);
}

void main(void) {
   pthread_t th1, th2, th3, th4;

   sem_init(&mutex, 0, 1);
   sem_init(&sem_lec, 0, 1);

   pthread_create(&th1, NULL, lector, NULL);
   pthread_create(&th2, NULL, escritor, NULL);
   pthread_create(&th3, NULL, lector, NULL);
   pthread_create(&th4, NULL, escritor, NULL);

   pthread_join(th1, NULL);
   pthread_join(th2, NULL);
   pthread_join(th3, NULL);
   pthread_join(th4, NULL);

   /* cerrar todos los semaforos */
   sem_destroy(&mutex);
   sem_destroy(&sem_lec);

   exit(0);
}

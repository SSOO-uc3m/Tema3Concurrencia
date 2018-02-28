/*Este programa sirve para crear un sistema de control de  lectores y escritores en un buffer. 
Puede haber m�ltiples lectores al tiempo, pero solo un escritor. 
* Compilar con gcc -lpthread 
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

const NUM_LECTORES = 5;
int dato = 1;       /* recurso */
int n_lectores = 0; /* numero de lectores */
sem_t sem_lec;      /* controlar el acceso n_lectores */
sem_t mutex;        /* controlar el acceso a dato */

void Lector(void *threadid) { /* codigo del lector */
 int tid = (intptr_t) threadid;
 
 while (1) {
   sem_wait(&sem_lec);
   n_lectores++;
   if (n_lectores == 1) sem_wait(&mutex);
   sem_post(&sem_lec);

   printf("Lector %d ha leido %d\n", tid,dato);  /* leer dato */

   sem_wait(&sem_lec);
   n_lectores--;
   if (n_lectores == 0) sem_post(&mutex);
   sem_post(&sem_lec);
   sleep(1);
 }
  pthread_exit(0);
}

void Escritor(void *threadid) {   /* codigo del escritor */
   int tid = (intptr_t) threadid;
   while (1) {
	    sem_wait(&mutex);    
        dato += 2;     /* modificar el recurso */
        sem_post(&mutex);
        printf("Soy el escritor y he modificado el valor a %d\n",dato);
        sleep(1);
   }
   pthread_exit(0);
}

int main(void) {
   pthread_t thLectores[NUM_LECTORES], thEscritor;
   int i = 0;
   sem_init(&mutex, 0, 1);
   sem_init(&sem_lec, 0, 1);
  
  pthread_create(&thEscritor, NULL, (void *)Escritor, (void *) (intptr_t) i );
  
  
  for ( i=0; i < NUM_LECTORES;i++){
    pthread_create(&thLectores[i], NULL, (void *)Lector, (void *) (intptr_t) i);
  }
  
    pthread_join(thEscritor, NULL);
    for ( i=0; i < NUM_LECTORES;i++){
    pthread_join(thLectores[i], NULL);
  }
  /* cerrar todos los semaforos */
  sem_destroy(&mutex);
  sem_destroy(&sem_lec);

  exit(0);
}

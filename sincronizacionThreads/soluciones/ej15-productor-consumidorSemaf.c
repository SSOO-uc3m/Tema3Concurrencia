/*Este programa sirve para crear un sistema de productor consumidor. 
* Ambos comparten un buffer que inicialmente se arranca vacio. 
* El productor pone elementos en el buffer y el consumidor los retira.  Si se llena, el productor debe parar. Si se vacía el consumido se debe parar.
* Compilar con gcc -lpthread ej15-productor-consumidorSemaf.c
* Jesús Carretero
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_BUFFER          1024    /* tamanio del buffer */
#define DATOS_A_PRODUCIR   10000    /* datos a producir */ 

sem_t elementos;  		/* elementos en el buffer */
sem_t huecos;    		/* huecos en el buffer */
int buffer[MAX_BUFFER];  	/* buffer comun */

int main(void)
{
   pthread_t th1, th2;  /* identificadores de threads */

   /* inicializar los semaforos */
   sem_init(&elementos, 0, 0);
   sem_init(&huecos, 0, MAX_BUFFER);
/* crear los procesos ligeros */
   pthread_create(&th1, NULL, Productor, NULL);
   pthread_create(&th2, NULL, Consumidor, NULL);  

   /* esperar su finalizacion */
   pthread_join(th1, NULL);
   pthread_join(th2, NULL);

   sem_destroy(&huecos);
   sem_destroy(&elementos);
   exit(0);
}

void Productor(void)   /* codigo del productor */
{
   int pos = 0;  /* posicion dentro del buffer */
   int dato;     /* dato a producir */
   int i;

   for(i=0; i < DATOS_A_PRODUCIR; i++ )   {
      dato = i;           /* producir dato */
      sem_wait(&huecos);  /* un hueco menos */
      buffer[pos] = i;
      pos = (pos + 1) % MAX_BUFFER;
      sem_post(&elementos);  /* un elemento mas */
  }
  pthread_exit(0);
}

void Consumidor(void)  /* codigo del Consumidor */
{
   int pos = 0;
   int dato;
   int i;

   for(i=0; i < DATOS_A_PRODUCIR; i++ ) {
      sem_wait(&elementos);    /* un elemento menos */
      dato = buffer[pos];
      pos = (pos + 1) % MAX_BUFFER;
      sem_post(&huecos);    /* un hueco mas */
      /* cosumir dato */
   }
   pthread_exit(0);
}

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define MAX_BUFFER          1024    /* tamanio del buffer */
#define DATOS_A_PRODUCIR  100000    /* datos a producir */ 

sem_t elementos;  		/* elementos en el buffer */
sem_t huecos;    		/* huecos en el buffer */
int buffer[MAX_BUFFER];  	/* buffer comun */


void *productor(void *arg)   /* codigo del productor */
{
	int pos = 0;  /* posicion dentro del buffer */
	int dato;     /* dato a producir */
	int i;

	for(i=0; i < DATOS_A_PRODUCIR; i++ )   {
		dato = i;           /* producir dato */
		sem_wait(&huecos);  /* un hueco menos */
		printf("P: agnado dato %d en %d\n",dato,pos);
		buffer[pos] = i;
		pos = (pos + 1) % MAX_BUFFER;
		sem_post(&elementos);  /* un elemento mas */

	}
	pthread_exit(0);
}

void *consumidor(void *arg)  /* codigo del Consumidor */
{
	int pos = 0;
	int dato;
	int i;

	for(i=0; i < DATOS_A_PRODUCIR; i++ ) {
		sem_wait(&elementos);    /* un elemento menos */
		dato = buffer[pos];
		printf("\tC: consumo elemento %d de %d\n",dato,pos);
		pos = (pos + 1) % MAX_BUFFER;
		sem_post(&huecos);    /* un hueco mas */
		/* cosumir dato */
	}
	pthread_exit(0);
}



void main(void)
{
	pthread_t th1, th2;  /* identificadores de threads */

	/* inicializar los semaforos */
	sem_init(&elementos, 0, 0);
	sem_init(&huecos, 0, MAX_BUFFER);
	
	/* esperar su finalizacion */
	pthread_create(&th1, NULL, productor, NULL);
   	pthread_create(&th2, NULL, consumidor, NULL);  

      	/* esperar su finalizacion */
      	pthread_join(th1, NULL);
        pthread_join(th2, NULL);

	sem_destroy(&huecos);
	sem_destroy(&elementos);
	exit(0);
}


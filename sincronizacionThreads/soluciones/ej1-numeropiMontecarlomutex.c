/* Programa que crea 10 threads.
 * Cada uno de ellos calcula el valor del número PI usando el método de Montecarlo y lo almacena  en la posición que le corresponde de en un array.
 * Cuando han terminado todos los threads el programa principal calcula la media de los valores de pi almacenados en el array
 * Utilizamos mutex y var. condicionales para coordinar la lectura del parámtro pasado al thread.
 * Compilar con gcc -lpthread -lm  
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define RADIO 5000
#define PUNTOS 1000000
#define NUM_HILOS 10
#define true 1
#define false 0

//Variable global compartida por todos los threads, incluido el main
float valoresPIthreads[NUM_HILOS]; 
pthread_mutex_t mtx;
pthread_cond_t varcond;
int flagGuardado=false;

void *calcula_pi (void *kk);
float montecarlo(float pi);

int main() {
	
	pthread_t thread[NUM_HILOS];
	int i;
	float *valorpi=0, suma=0, media=0;

	pthread_cond_init (&varcond, NULL);
	pthread_mutex_init (&mtx, NULL);
	
	
	for (i=1;i<NUM_HILOS+1;i++) {
	  pthread_create(&thread[i],NULL,calcula_pi,&i);

	  pthread_mutex_lock(&mtx);
	  while (!flagGuardado){
	      pthread_cond_wait (&varcond, &mtx);
	  }
      flagGuardado=false;
	  pthread_mutex_unlock(&mtx);

	  printf ("Creado thread %d\n",i);
	}
	for (i=1;i<NUM_HILOS+1;i++) {
		pthread_join(thread[i],NULL);
	}
	for (i=1;i<NUM_HILOS+1;i++) {
		printf("Valor del thread %d: %f\n",i,valoresPIthreads[i]);
		suma=suma+valoresPIthreads[i];
	}
	media=suma/(NUM_HILOS);
	printf("El valor medio de Pi obtenido es: %f\n",media);
}

void *calcula_pi (void *idthread)
{
	int numthread;
	float pi =0;
	
    pthread_mutex_lock (&mtx); 
    // seccion critica
    numthread=*((int *)idthread);
    flagGuardado=true;
     // liberamos el candado
    pthread_cond_signal (&varcond);
    pthread_mutex_unlock (&mtx);

    printf ("th %d calculando\n", numthread);
	valoresPIthreads[numthread]=montecarlo(pi);

	pthread_exit(&pi);
}

float montecarlo (float pi){
	int j, y=0, x=0, cont=0;
	float  h=0;
	srandom((unsigned)pthread_self());
	for (j=0;j<PUNTOS;j++) {
		y=(random()%((2*RADIO)+1)-RADIO);
		x=(random()%((2*RADIO)+1)-RADIO);
		h=sqrt((x*x)+(y*y));
		if ( h<=RADIO ) cont++;
	}	
	return (cont*4)/(float)PUNTOS;
}


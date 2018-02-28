#include <pthread.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>


#define NUMTHREADS 6
//Primero pasan 3 y luego otros 3
#define THBARRERA 3

pthread_t th[NUMTHREADS];
pthread_barrier_t mi_barrera;

void * sync_carrera (void * data)
{
   int espera=random()%5;
   printf ("Espera %d thread %d\n", espera, (int)pthread_self());	
   sleep(espera);

   pthread_barrier_wait(&mi_barrera);
   printf ("Paso la mi_barrera thread %d\n", (int) pthread_self());	
   pthread_exit(NULL);
}

int main (int argc, char ** argv)
{
  int i;

  pthread_barrier_init(&mi_barrera, NULL, THBARRERA );

  for (i=0; i<NUMTHREADS; i++)
    pthread_create(&th[i], NULL, sync_carrera, NULL);

  printf ("THS creados\n");

  for (i=0; i<NUMTHREADS; i++) {
    pthread_join(th[i], NULL);  
   // Espera por un thread concreto. Si el orden de finalización no es el de creación 
   // (ej. termina el 1 y luego el 0) espera por el 0 hasta que termine y luego espera por el 1)
    printf ("Fin th:%d\n", (int)th[i]);
  }
  pthread_barrier_destroy(&mi_barrera);
  printf ("FIN\n");
}


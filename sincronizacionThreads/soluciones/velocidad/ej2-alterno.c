/* fichero alterno.c THREADS
Crea 2 threads que ejecutan de forma alterna.
 compilar con  gcc -lpthread alterno.c
José Manuel Pérez Lobato
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const int NUMERO_MAX = 200;
int dato_compartido = 0;
pthread_mutex_t mtx;
pthread_cond_t esperaPares;
pthread_cond_t esperaImpares;

enum numeros {PARES, IMPARES};
enum numeros turno= PARES;

void *hiloImpares(void *arg) {
   int cont=1;

   while (cont <=NUMERO_MAX){
      pthread_mutex_lock (&mtx);
      while (turno!=IMPARES) 
         pthread_cond_wait(&esperaPares, &mtx);
      printf ("thImpares:%d\n", dato_compartido++);
      cont=cont+2;
      turno = PARES;
      pthread_cond_signal(&esperaImpares);
      pthread_mutex_unlock (&mtx);
   }
   pthread_exit(0);
}

void *hiloPares(void *arg) {
   int cont=2;

   while (cont <=NUMERO_MAX){
      pthread_mutex_lock (&mtx);
      while (turno!=PARES) 
	pthread_cond_wait(&esperaImpares, &mtx);
      printf ("thPares:%d\n", dato_compartido++);
      cont=cont+2;
      turno = IMPARES;
      pthread_cond_signal(&esperaPares);
      pthread_mutex_unlock (&mtx);
  }
   pthread_exit(0);
}

int main(){
    int i;
    pthread_t idth[2];
    clock_t t_ini, t_fin;
    double segs;
    t_ini = clock();
    
    pthread_mutex_init (&mtx, NULL);
    pthread_cond_init (&esperaPares, NULL);
    pthread_cond_init (&esperaImpares, NULL);

    pthread_create(&idth[0],NULL,hiloImpares,NULL);
    pthread_create(&idth[1],NULL,hiloPares,NULL);
    // Espero la finalizacion del thread
    for (i=0; i<2; i++) 
      pthread_join(idth[i],NULL);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&esperaPares);
    pthread_cond_destroy(&esperaImpares);

    t_fin = clock();
    segs  = (double) (t_fin - t_ini);
    printf("%.2f segundos\n", segs / 1000 );

    return(0);
}


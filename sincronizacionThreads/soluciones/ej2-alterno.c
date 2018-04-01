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

const int NUMERO_MAX = 10;

pthread_attr_t attr;
pthread_mutex_t mtx;
pthread_cond_t varcond;
enum numeros {PARES, IMPARES};

enum numeros turno= IMPARES;

void *hiloImpares(void *arg) {
   int cont=1;

   while (cont <=NUMERO_MAX){
      pthread_mutex_lock (&mtx);
   while (turno!=IMPARES) 
      pthread_cond_wait(&varcond, &mtx);
   printf ("thImpares:%d\n", cont);
   cont=cont+2;
   turno = PARES;
   pthread_cond_signal(&varcond);
   pthread_mutex_unlock (&mtx);
}
   pthread_exit(0);
}

void *hiloPares(void *arg) {
   int cont=2;

   while (cont <=NUMERO_MAX){
    pthread_mutex_lock (&mtx);
    while (turno!=PARES) 
	pthread_cond_wait(&varcond, &mtx);
    printf ("thPares:%d\n", cont);
    cont=cont+2;
    turno = IMPARES;
    pthread_cond_signal(&varcond);
   pthread_mutex_unlock (&mtx);
}
   pthread_exit(0);
}

int main(){
    int i;
    pthread_t idth[2];

    pthread_mutex_init (&mtx, NULL); 
    pthread_attr_init(&attr);
 
    pthread_create(&idth[0],&attr,hiloImpares,NULL);
    pthread_create(&idth[1],&attr,hiloPares,NULL);
  // Espero la finalización del thread
    for (i=0; i<2; i++) 
      pthread_join(idth[i],NULL);
    return(0);
}


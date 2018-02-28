/* fichero alterno.c
THREADS
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

pthread_attr_t attr;
pthread_t idth[10];
pthread_mutex_t mtx;
pthread_cond_t varcond;
int turno=1;

void *hilo1(void *num) {
   int cont=1;

while (cont <=10){
   pthread_mutex_lock (&mtx);
    while (turno!=1) pthread_cond_wait(&varcond, &mtx);
    printf ("th1:%d\n", cont);
    cont=cont+2;
    turno = 2;
    pthread_cond_signal(&varcond);
   pthread_mutex_unlock (&mtx);
} 
   pthread_exit(0);
} 
void *hilo2(void *num) {
   int cont=2;

while (cont <=10){
   pthread_mutex_lock (&mtx);
    while (turno!=2) pthread_cond_wait(&varcond, &mtx);
    printf ("th2:%d\n", cont);
    cont=cont+2;
    turno = 1;
    pthread_cond_signal(&varcond);
   pthread_mutex_unlock (&mtx);
} 
   pthread_exit(0);
} 

int main(){
   int i;
   
    pthread_mutex_init (&mtx, NULL); 
    pthread_attr_init(&attr);

    pthread_create(&idth[0],&attr,hilo1,&i);
    pthread_create(&idth[1],&attr,hilo2,&i);
  // Espero la finalización del thread
    for (i=0; i<2; i++) 
      pthread_join(idth[i],NULL);
    return(0);
}


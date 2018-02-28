/* Este programa crea 2 threads, uno escribe los números pares y otro los impares del 1 al 10. 
Su ejecución es alterna para que salgan ordenados
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX         10      /* Numero máximo*/
#define TRUE        1      
#define FALSE       0      

pthread_mutex_t mutex;     /* mutex para controlar el acceso al  
                           buffer compartido */
pthread_cond_t esperaPares;   /* controla la espera de los pares*/
pthread_cond_t esperaImpares;   /* controla la espera de los impares*/
int turnoPares=TRUE;
int turnoImpares=FALSE;

void  *pares(void *kk)  {   /* codigo del que escribe los pares */
   int i;

    for(i=0; i <= MAX; i+=2 )  {
        pthread_mutex_lock(&mutex);        /* acceder al buffer */
        while (turnoPares == FALSE)  
            pthread_cond_wait(&esperaPares, &mutex); /* se bloquea */
 	    printf ("Pares: %d\n", i);
	    turnoImpares=TRUE;
	    turnoPares=FALSE;
        pthread_cond_signal(&esperaImpares);   /* buffer no vacio */
        pthread_mutex_unlock(&mutex);
    }
    printf ("FIN PARES\n");
    pthread_exit(0);
}
void  *impares(void *kk)  {   /* codigo del que escribe los pares */
   int i;

    for(i=1; i <= MAX; i=i+2 )  {
        pthread_mutex_lock(&mutex);        /* acceder al buffer */
        while (turnoImpares == FALSE)  
            pthread_cond_wait(&esperaImpares, &mutex); /* se bloquea */
 	    printf ("Impares: %d\n", i);
	    turnoImpares=FALSE;
	    turnoPares=TRUE;
        pthread_cond_signal(&esperaPares);   /* buffer no vacio */
        pthread_mutex_unlock(&mutex);
    }
    printf ("FIN IMPARES\n");
    pthread_exit(0);
}


int main(int argc, char *argv[]){
    pthread_t th1, th2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&esperaPares, NULL);
    pthread_cond_init(&esperaImpares, NULL);
    pthread_create(&th1, NULL, pares, NULL);
    pthread_create(&th2, NULL, impares, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&esperaPares);
    pthread_cond_destroy(&esperaImpares);

    exit(0);
}

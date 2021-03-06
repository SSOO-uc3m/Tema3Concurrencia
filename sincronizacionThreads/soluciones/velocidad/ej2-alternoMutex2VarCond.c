/* Este programa crea 2 threads, uno escribe los números pares y otro los impares del 1 al 10. 
Su ejecución es alterna para que salgan ordenados
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

const int NUMERO_MAX = 200;      /* Numero máximo*/
enum numeros {PARES, IMPARES};
enum numeros turno = PARES;

pthread_mutex_t mutex;     /* mutex para controlar el acceso al  
                           buffer compartido */
pthread_cond_t esperaPares;   /* controla la espera de los pares*/
pthread_cond_t esperaImpares;   /* controla la espera de los impares*/


void  *pares(void *arg)  {   /* codigo del que escribe los pares */
   int i;

    for(i=0; i <= NUMERO_MAX; i+=2 )  {
        pthread_mutex_lock(&mutex);        /* acceder al buffer */
        while (turno == IMPARES)  
            pthread_cond_wait(&esperaPares, &mutex); /* se bloquea */
 	    printf ("Pares: %d\n", i);
	    turno = IMPARES;
        pthread_cond_signal(&esperaImpares);   /* buffer no vacio */
        pthread_mutex_unlock(&mutex);
    }
    printf ("FIN PARES\n");
    pthread_exit(0);
}
void  *impares(void *kk)  {   /* codigo del que escribe los pares */
   int i;

    for(i=1; i <= NUMERO_MAX; i=i+2 )  {
        pthread_mutex_lock(&mutex);        /* acceder al buffer */
        while (turno == PARES)
            pthread_cond_wait(&esperaImpares, &mutex); /* se bloquea */
 	    printf ("Impares: %d\n", i);
	    turno = PARES;
        pthread_cond_signal(&esperaPares);   /* buffer no vacio */
        pthread_mutex_unlock(&mutex);
    }
    printf ("FIN IMPARES\n");
    pthread_exit(0);
}


int main(int argc, char *argv[]){
    pthread_t th1, th2;
    clock_t  t_ini, t_fin;
    double segs;

    t_ini = clock();
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

    t_fin = clock();
    segs = (double) (t_fin- t_ini);
    printf("%.2f segundos\n", segs / 1000);

    exit(0);
}

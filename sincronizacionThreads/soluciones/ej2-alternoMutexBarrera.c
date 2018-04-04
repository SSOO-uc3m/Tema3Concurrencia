/* Este programa crea 2 threads, uno escribe los números pares y otro los impares del 1 al 10. Su ejecución es alterna para que salgan ordenados
 * Ademas incorpora una barrera para que no se escriba el FIN hasta que los 2 threads no hayan terminado de escribir numeros
 * José Manuel Pérez Lobato */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

const int NUMERO_MAX = 200;      /* Numero máximo*/
enum numeros {PARES, IMPARES};

pthread_mutex_t mutex;     /* mutex para controlar al turno compartido */
pthread_cond_t espera;   /* controla la espera*/

enum numeros turno = PARES;

//para la barrera
pthread_mutex_t mtx;     /* mutex para controlar el acceso a la barrera*/
pthread_cond_t esperaBarrera;   /* controla la espera de la barrera */
int hilosespera=0;

void barrera(){
        pthread_mutex_lock(&mtx);        /* acceder al buffer */
	hilosespera++;
        while (hilosespera<2 )
            pthread_cond_wait(&esperaBarrera, &mtx); /* se bloquea */
        pthread_cond_signal(&esperaBarrera);
        pthread_mutex_unlock(&mtx);
}


void  *pares(void *arg)  {   /* codigo del que escribe los pares */
   int i;

    for(i=2; i <= NUMERO_MAX; i=i+2 )  {
        pthread_mutex_lock(&mutex);        /* acceder al buffer */
        while (turno == IMPARES)
            pthread_cond_wait(&espera, &mutex); /* se bloquea */
 	printf ("Pares: %d\n", i);
	turno=IMPARES;
        pthread_cond_signal(&espera);
        pthread_mutex_unlock(&mutex);
    }
    barrera();
    printf ("FIN PARES\n");
    pthread_exit(0);
}
void  *impares(void *arg)  {   /* codigo del que escribe los pares */
   int i;

    for(i=1; i <= NUMERO_MAX; i=i+2 )  {
        pthread_mutex_lock(&mutex);        /* acceder al buffer */
        while (turno == PARES)
            pthread_cond_wait(&espera, &mutex); /* se bloquea */
 	printf ("Impares: %d\n", i);
	turno = PARES;
        pthread_cond_signal(&espera);   /* buffer no vacio */
        pthread_mutex_unlock(&mutex);
    }
    barrera();
    printf ("FIN IMPARES\n");
    pthread_exit(0);
}


int main(int argc, char *argv[]){
    pthread_t th1, th2;

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&esperaBarrera, NULL);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&espera, NULL);

    pthread_create(&th1, NULL, pares, NULL);
    pthread_create(&th2, NULL, impares, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&espera);

    exit(0);
}


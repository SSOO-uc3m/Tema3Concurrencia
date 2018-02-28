/* Este programa crea 1 thread para el famoso que firma y otro para los fans que van llegando.
 * Los threads de los fans los crea aletoriamente cada x segundos.
 * El famoso sólo firma un autografo de cada vez y en la sala no caben mas de AFOROMAX fans y si hay menos de FANSMIN el famoso no firma.
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define AFOROMAX 20      /* Numero maximo de fans*/
#define FANSMIN 5      /* Numero minimo de fans*/
#define TRUE 1      
#define FALSE 0      

pthread_mutex_t mutex;     /* mutex para controlar el acceso al buffer compartido */
pthread_cond_t famoso_durmiendo;   /* controla la espera del famoso*/
pthread_cond_t autografo;   /* controla la espera de los fans*/

int ocupacion = 0;
int firmado = 0;

void *famoso(void *kk)  {   
   
	int i;

	while(1) {
		pthread_mutex_lock(&mutex);        /* acceder al contador */
		while (ocupacion < FANSMIN) {
			pthread_cond_wait(&famoso_durmiendo, &mutex); /* se bloquea */
		}
		printf("FAMOSO FIRMA. Ocupacion actual: %d\n", ocupacion);
		firmado++;
		pthread_cond_signal(&autografo);   
		pthread_mutex_unlock(&mutex);
		sleep(random() % 2);
	}
	
	pthread_exit(0);
}

void *fan(void *kk)  {   
   
	int i;

	pthread_mutex_lock(&mutex);        /* acceder al contador */
	if (ocupacion != AFOROMAX) {
		ocupacion++;
		printf("fan espera: %u, Ocupacion actual: %d\n", (unsigned) pthread_self(), ocupacion);
		pthread_cond_signal(&famoso_durmiendo);   
		while (firmado == 0) {
			pthread_cond_wait(&autografo, &mutex); /* se bloquea */
		}
		firmado--;
		ocupacion--;
		printf("FIN fan atendido: %u, Quedan %d fans dentro\n", (unsigned) pthread_self(), ocupacion);
	}
	else {
		printf("FIN fan sin atender: %u\n", (unsigned) pthread_self());
	}
	pthread_mutex_unlock(&mutex);

	pthread_exit(0);
}

int main(int argc, char *argv[]) {
    
	int i;
    pthread_t th1, th2;
    pthread_attr_t attrfan;

    pthread_attr_init(&attrfan);
    pthread_attr_setdetachstate(&attrfan, PTHREAD_CREATE_DETACHED);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&famoso_durmiendo, NULL);
    pthread_cond_init(&autografo, NULL);

    pthread_create(&th1, NULL, famoso, NULL);
    for (i = 0; i < 40; i++) {
      pthread_create(&th2, &attrfan, fan, NULL);
	  sleep(random() % 1);
    }
    
    pthread_join(th1, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&famoso_durmiendo);
    pthread_cond_destroy(&autografo);

    return 0;
}


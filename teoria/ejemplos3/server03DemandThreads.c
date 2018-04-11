#include "peticion.h"
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

sem_t snhijos;

const int MAX_PETICIONES = 5;

void *receptor (void* param);
void *servicio (void* p);
void copiar_peticion(peticion_t *origen, peticion_t *destino);

int main()  {

     time_t t1,t2;
     double dif;
     pthread_t thr;

     t1 = time(NULL);

     sem_init(&snhijos, 0, 0);
     pthread_create(&thr, NULL, receptor, NULL);
     pthread_join(thr, NULL);
     sem_destroy(&snhijos);

     t2 = time (NULL);

     dif = difftime(t2,t1);
     printf("Tiempo: %lf\n",dif);

    return 0;
 }

void *receptor(void *param){
    int nservicio = 0, i;
    peticion_t p;
    p.id = 0;
    p.tipo = 0;
    strcpy(p.url,"localhost");

    pthread_t th_hijo;

    for(i=0;i<MAX_PETICIONES;i++){
        p.id = i;
	recibir_peticion(&p);
	nservicio++;
	pthread_create(&th_hijo, NULL, servicio, &p);
    }
    printf("Esperando Threads\n");
    for(i=0; i<nservicio;i++) {
	sem_wait(&snhijos);
	printf("Termina un thread%d\n");
    }

    pthread_exit(0);
    return NULL;

}

void *servicio (void *p){
    peticion_t pet;
    copiar_peticion(&pet, (peticion_t*)p);
    printf("iniciando servicio peticion \n");
    responder_peticion(&pet);
    sem_post(&snhijos); // aviso al hilo padre que termine 

    printf("Terminando servicio peticion \n");
    pthread_exit(0);
    return NULL;

}

void copiar_peticion (peticion_t *origen, peticion_t *destino){
   memcpy(origen ,destino, sizeof(destino));
}

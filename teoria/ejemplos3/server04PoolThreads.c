#include "peticion.h"
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define MAX_BUFFER 128

const int MAX_PETICIONES = 5;
const int MAX_SERVICIO = 5;

peticion_t buffer [MAX_BUFFER];
int n_elementos = 0;
int pos_servicio = 0;

pthread_mutex_t mutex;
pthread_cond_t no_lleno;
pthread_cond_t no_vacio;

int fin = 0;

void *receptor (void* param);
void *servicio (void* param);


int main()  {

     time_t t1,t2;
     double dif;
     pthread_t thr;
     pthread_t ths[MAX_SERVICIO];
     int i;

     t1 = time(NULL);

     pthread_mutex_init(&mutex, NULL);
     pthread_cond_init(&no_lleno, NULL);
     pthread_cond_init(&no_vacio, NULL);

     pthread_create(&thr, NULL, receptor, NULL);

     for(i=0;i<MAX_SERVICIO;i++){
      	pthread_create(&ths[i], NULL, servicio, NULL);
     }

     pthread_join(thr, NULL);
     for (i=0;i<MAX_SERVICIO;i++) {
     	pthread_join(ths[i],NULL);
     }

     pthread_mutex_destroy(&mutex);
     pthread_cond_destroy(&no_lleno);
     pthread_cond_destroy(&no_vacio);

     t2 = time (NULL);

     dif = difftime(t2,t1);
     printf("Tiempo: %lf\n",dif);

    return 0;
 }

void *receptor(void *param){
    int nservicio = 0,
    i,
    pos = 0;

    peticion_t p;
    p.id = 0;
    p.tipo = 0;
    strcpy(p.url,"localhost");

     for (i=0;i<MAX_PETICIONES;i++){
          recibir_peticion(&p);
          pthread_mutex_lock(&mutex);
          while (n_elementos == MAX_BUFFER)
                      pthread_cond_wait(&no_lleno, &mutex);
          buffer[pos] = p;
          pos = (pos+1) % MAX_BUFFER;
          n_elementos++;
          pthread_cond_signal(&no_vacio); //despertamos a un hilo del pool
          pthread_mutex_unlock(&mutex);
     }

    fprintf(stderr,"Finalizando receptor\n");

    pthread_mutex_lock(&mutex);
    fin=1;
    pthread_cond_broadcast(&no_vacio);
    pthread_mutex_unlock(&mutex);

    fprintf(stderr, "Finalizado receptor\n");

    pthread_exit(0);

}

void *servicio (void *param){
      peticion_t p;

      for (;;)  {
           pthread_mutex_lock(&mutex);
           while (n_elementos == 0) {
                if (fin) {
                     fprintf(stderr,"Finalizando servicio\n");
                     pthread_mutex_unlock(&mutex);
                     pthread_exit(0);
                }
                pthread_cond_wait(&no_vacio, &mutex);
           } // while

           fprintf(stderr, "Sirviendo posicion %d\n", pos_servicio);
           p = buffer[pos_servicio];
           pos_servicio = (pos_servicio + 1) % MAX_BUFFER;
           n_elementos --;
           pthread_cond_signal(&no_lleno); // despertamos al receptor
           pthread_mutex_unlock(&mutex);

           responder_peticion(&p);

    }// bucle infinito

}



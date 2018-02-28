/*Este programa sirve para controlar un sistema de riego de 5 válvulas de riego y 3 entradas de agua.
 * Se debe cumplir la restricción de que nunca debe de haber abiertas un número de válvulas superior al número de entradas de agua abiertas

*/

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_VALVULAS  5
#define MAX_ENTRADAS  3

int entradasAbtas=0;
int valvulasAbtas=0;
int abrirentrada=0;
int cerrarentrada=0;

pthread_mutex_t me;
pthread_mutex_t mv;
pthread_cond_t abrirE;
pthread_cond_t cerrarE;
pthread_cond_t vValvula;


void * valvula (void *n){
  while(1) {
  //Si hay más valvulas abts que entradas espero 
    pthread_mutex_lock(&mv);
      while( entradasAbtas<=valvulasAbtas ) {
        pthread_cond_wait(&vValvula,&mv);
      }
      valvulasAbtas++; 
    pthread_mutex_unlock(&mv);
    sleep(3);
    pthread_mutex_lock(&mv);
      valvulasAbtas--;
      pthread_cond_signal(&vValvula);
    pthread_mutex_unlock(&mv);
    sleep(1);
  }
}

void * entrada (void *n){
  while(1) {
    pthread_mutex_lock(&me);
      //Espero hasta recibir una orden de abrir entrada
      while(abrirentrada==0 ) {
        pthread_cond_wait(&abrirE,&me);
      }
      abrirentrada=0;
      pthread_mutex_lock(&mv);
        entradasAbtas++;
        pthread_cond_signal(&vValvula);
      pthread_mutex_unlock(&mv);
    pthread_mutex_unlock(&me);
  // esperamos a que se ordene el cierre
    pthread_mutex_lock(&me);
      while(cerrarentrada==0 ) {
        pthread_cond_wait(&cerrarE,&me);
      }
      cerrarentrada=0;
      entradasAbtas--;
    pthread_mutex_unlock(&me);
  }
  pthread_exit(NULL);
}

int main() {
	int i;
        char resp[10];
        pthread_t identrada[MAX_ENTRADAS];
	pthread_t idvalvula[MAX_VALVULAS];
		
	pthread_mutex_init(&me,NULL);
	pthread_mutex_init(&mv,NULL);
	pthread_cond_init(&abrirE,NULL);
	pthread_cond_init(&cerrarE,NULL);
	pthread_cond_init(&vValvula,NULL);

  //Creo tantos threads que gestionan valvulas como valvulas disponibles
  for (i=0; i< MAX_VALVULAS; i++)	
    pthread_create(&idvalvula[i],NULL,valvula,NULL);
  for (i=0; i< MAX_ENTRADAS; i++)	
    pthread_create(&identrada[i],NULL,entrada,NULL);
  while(1) {
    printf ("Ahora hay %d entradas abiertas y %d valvulas abtas\n",entradasAbtas,valvulasAbtas);
    printf ("Si quieres abrir una entrada pulse A si quiere cerrar una valvula pulse C:");
    scanf ("%s", resp);
    if  (resp[0] == 'A' ) {
    //Señalizo para que se abra una entrada
      pthread_mutex_lock(&me);
        abrirentrada=1;
        pthread_cond_signal(&abrirE);
      pthread_mutex_unlock(&me);
    }
    if  (resp[0] == 'C' ) {
    //Señalizo para que se cierre una entrada
      pthread_mutex_lock(&me);
        cerrarentrada=1;
        pthread_cond_signal(&cerrarE);
      pthread_mutex_unlock(&me);
    }
  }
  pthread_mutex_destroy(&me);
  pthread_mutex_destroy(&mv);
}

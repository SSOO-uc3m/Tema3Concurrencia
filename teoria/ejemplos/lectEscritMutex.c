#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int dato = 5;                   /* recurso */
int n_lectores = 0;             /* numero de lectores */
pthread_mutex_t mutex;          /* controlar el acceso a dato */
pthread_mutex_t mutex_lectores;     /* controla acceso n_lectores */
pthread_cond_t no_lectores;

void *escritor(void *arg) {   /* codigo del escritor */
   
   srand(time(NULL));
   int i;
   for(i= rand() % 100;i>0;i--){
     pthread_mutex_lock(&mutex);
     dato = dato + 2;     /* modificar el recurso */
     printf("P: escribe dato %d\n",dato);
     pthread_mutex_unlock(&mutex);
   }
   pthread_exit(0);
}

void *lector(void *arg) { /* codigo del lector */
   srand(time(NULL));
  int i;
  
   for(i = rand() % 100; i>0; i--){
     pthread_mutex_lock(&mutex_lectores);
     n_lectores++;
     if (n_lectores == 1) pthread_mutex_lock(&mutex);
     pthread_mutex_unlock(&mutex_lectores);
     printf("\tL:lee dato %d (num lectores %d)\n",dato,n_lectores); /* leer dato */

     pthread_mutex_lock(&mutex_lectores);
     n_lectores--;
     if (n_lectores == 0) pthread_mutex_unlock(&mutex);
     pthread_mutex_unlock(&mutex_lectores);
   }
   pthread_exit(0);
}

int main(int argc, char *argv[])  {
    pthread_t th1, th2, th3, th4;
    int i;
    pthread_t thLectores[10];
    pthread_t thEscritores[3];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&no_lectores, NULL);

    for (i=0; i< 10; i++){
     pthread_create(&thLectores[i], NULL, lector, NULL);
    }
    for (i=0; i< 3; i++){
     pthread_create(&thEscritores[i], NULL, escritor, NULL);
    } 
    //pthread_create(&th1, NULL, lector, NULL);
    //pthread_create(&th2, NULL, escritor, NULL);
    //pthread_create(&th3, NULL, lector, NULL);
    //pthread_create(&th4, NULL, escritor, NULL);

    //pthread_join(th1, NULL);
    //pthread_join(th2, NULL);
    //pthread_join(th3, NULL);
    //pthread_join(th4, NULL);
    for(int i=0; i< 10; i++){
       pthread_join(thLectores[i], NULL);
    }

    for(int i=0; i< 3; i++){
       pthread_join(thEscritores[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&no_lectores);

  exit(0);
}


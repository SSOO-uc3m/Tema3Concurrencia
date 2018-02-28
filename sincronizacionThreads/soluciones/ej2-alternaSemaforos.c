
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>



int dato_compartido = 0;
sem_t par,impar;

void pares(void)
{  int i;
   for(i=0; i < 100; i++ ) {
        sem_wait(&par);
        printf("Thread1 = %d \n", dato_compartido++);
        sem_post(&impar);
   }
}

void impares(void)
{  int i;
   for(i=0; i < 100; i++ ) {
       sem_wait(&impar);
           printf("Thread2 = %d \n", dato_compartido++);
           sem_post(&par);
   }
}

int main(void) {
    pthread_t th1, th2;
    sem_init(&par,0,1);
    sem_init(&impar,0,0);
    pthread_create(&th1, NULL, pares, NULL);
    pthread_create(&th2, NULL, impares, NULL);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    sem_destroy(&par);
    sem_destroy(&impar);
}


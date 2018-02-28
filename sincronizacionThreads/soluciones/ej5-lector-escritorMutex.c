/*Programa de las transparencias de lectores escritores hecho con mutex */
/*Jose Manuel Perez Lobato*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * Lector(void * p);
void * Escritor(void * p);

int dato=5; //recurso

pthread_mutex_t mutex;
pthread_cond_t leer,escribir;

int leyendo=0,escribiendo=0;

int main()
{
    pthread_t  l1,l2,e1,e2;
    
    int i;
    

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&leer,NULL);
    pthread_cond_init(&escribir,NULL);
    srand(time(NULL));
    
    //lanzamos los procesos ligeros concurrentemente
    pthread_create(&e1,NULL,Escritor,(void *)1);
    pthread_create(&e2,NULL,Escritor,(void *)2);
    pthread_create(&l1,NULL,Lector,(void *)1);
    pthread_create(&l2,NULL,Lector,(void *)2);

    //esperamos por la terminacion todos ellos
    pthread_join(l1,NULL);
    pthread_join(l2,NULL);
    pthread_join(e1,NULL);
    pthread_join(e2,NULL);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&leer);
    pthread_cond_destroy(&escribir);
    
}


void * Lector(void *p)
{	
    int i;
    i=(int)p;
    while(1) //lee eternamente
    {
	pthread_mutex_lock(&mutex);    
        if(escribiendo) 
	    pthread_cond_wait(&leer,&mutex);
	leyendo++;
	pthread_mutex_unlock(&mutex);
	
	//lecturas simultaneas del recurso compartido
        printf("Soy el lector %d y leo el dato %d\n", i, dato);
	sleep(1); //tardo 1s en leer

	pthread_mutex_lock(&mutex);    
	leyendo--;
	if(!leyendo)  //Si no hay ningun lector mas, damos paso a los escritores
	    pthread_cond_signal(&escribir);
	pthread_mutex_unlock(&mutex);
	
	sleep(rand()%5);  //espero un tiempo aleatorio
    }
    
}


void * Escritor(void * p)
{
   int i;
   i=(int)p;
   while(1)
   {
	pthread_mutex_lock(&mutex);    
	while(leyendo||escribiendo)
	    pthread_cond_wait(&escribir,&mutex);
	escribiendo++;
	pthread_mutex_unlock(&mutex);
	
	//acceso en exclusiva al recurso
        sleep(1); //tardo 1 sg en escribir
        dato+=2;
        printf("Soy el escritor %d y he modificado el valor a %d\n", i,dato);
        
	pthread_mutex_lock(&mutex);
	escribiendo--;
	pthread_cond_broadcast(&escribir); 
	pthread_cond_broadcast(&leer);
	pthread_mutex_unlock(&mutex);

        sleep(rand()%5);  //espero un tiempo aleatorio
   }
}



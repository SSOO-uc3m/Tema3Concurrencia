/*
El proceso principal creará 10 procesos ligeros. Cada uno de estos procesos ligeros
establece un rango (inicio… fin) en el que guardar valores en el vector. 
Cuando cada trabajador termina de guardar valores, incrementa b.n y pregunta si b.n es igual a N:
•	Si el proceso no es el último: (n<=N) entonces el proceso se duerme.
•	Si el proceso es el último (n>N) entonces el proceso despierta a todos los procesos ligeros dormidos.
El proceso principal espera al final a los procesos ligeros.


*/#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10
#define TAMANIO 1024

void *trabajador(void *arg);

int vector[TAMANIO];
struct b_s {
  int n;
  pthread_mutex_t m;
  pthread_cond_t ll;
} b;


int main(void) {
  pthread_t hilo[N];
  int i;

  	
  b.n = 0;
  pthread_mutex_init(&b.m, NULL);
  pthread_cond_init(&b.ll, NULL);
  par=0; impar=1;
  
  for(i=0; i<N; i++)
    pthread_create(&hilo[i],
                 NULL, trabajador, 
                 (void *)&i);
  
  for(i=0; i<N; i++)
    pthread_join(hilo[i], NULL);

  pthread_cond_destroy(&b.ll);
  pthread_mutex_destroy(&b.m);

  return 0;
}

void *trabajador(void *arg) {
int inicio=0, fin=0, i;


id  = *(int *)arg; 
inicio =(id)*(TAMANIO/N);

fin = (id+1)*(TAMANIO/N);

for(i=inicio; i<fin; i++) {
	vector[i] = id;
}
pthread_mutex_lock(&b.m); 
b.n++; 
if (N<=b.n) {		pthread_cond_broadcast(&b.ll);
} else {
	pthread_cond_wait(&b.ll, &b.m); 
}
pthread_mutex_unlock(&b.m); 
	
return 0;
}

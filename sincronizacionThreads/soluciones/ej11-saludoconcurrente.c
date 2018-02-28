//THREADS
/* Realizar un programa que declare una función imprimir y
 que le pase como parámetros 1 string a imprimir.
A continuación el programa principal debe preparar los parámetros con 2 string "hola" y "mundo" y lanzar 2 threads que intenten imprimir "hola mundo" en ese orden N veces y terminar. */
//Jesús Carretero

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define N 3

pthread_t thread1, thread2; /* Declaración de los threads */
pthread_attr_t attr; /*atributos de los threads*/ 
pthread_mutex_t impresor=PTHREAD_MUTEX_INITIALIZER;


/* Definición de las función imprimir */
void *imprimir (void *arg) 
{ 
  char a[12];

  pthread_mutex_lock (&impresor);
  strcpy(a, (char*)arg);

  printf("%s ",a); 
  pthread_mutex_unlock (&impresor);

  pthread_exit (NULL); 
}


/*Función main*/ 
int main (void) 
{ 
  char cadena_hola[]="Hola ";
  char cadena_mundo[]="mundo \n";
  int i;

  pthread_attr_init (&attr); 

  for (i=1; i<=N; i++) {
     pthread_create(&thread1, &attr, imprimir, (void *)cadena_hola); 
     pthread_create(&thread2, &attr, imprimir, (void *)cadena_mundo); 
  }

  pthread_exit (NULL); 
} 

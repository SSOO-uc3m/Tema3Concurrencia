/* compilar como gcc -pthread -o main main.c*/
/* taskset 0x01 para ejecutarlo en un único procesador */
//#define _GNU_SOURCE
#include <stdio.h>  // printf
#include <pthread.h>// pthread_create,pthread_exit
#include <stdlib.h> // exit

#define true 1
#define false 0
#define NUM_SUMADO 10000
#define NUM_VECES 10000

/** 
* Variable global con el valor acumulado donde se realiza la suma.
* Hace las veces de variable compartida entre las dos hebras.
*/
long suma_total = 0;
static volatile long turno=0;

/**
* función que devuelve acumulador + n. Hace la
* suma de uno en uno con un for.
*/
long sumaN(long acumulador, int n) {
    int i;
    long total = acumulador;

    for (i=0;i<n;i++){
       total += 1;
        
    }
    
    return total;
}

void entradaSeccionCritica(long numHebra){
    //pthread_yield();
    while(turno !=numHebra);
    // Nos toca!
}// entradaSeccionCritica

void salidaSeccionCritica(long numHebra){
    int otraHebra = 1-numHebra;
    turno = otraHebra;
}





/**
* La función a ser ejecutado a través de una hebra. Llama
* NUM_VECES a sumaN para sumar NUMERO_SUMADO al atributo _suma.
*/
void *run(void *threadid){
    int i;
    long tid = (long)threadid; // keep book of thread's id
    //printf("This is thread #%ld!\n", tid);

    for (i=1;i<= NUM_VECES;i++){     
        entradaSeccionCritica(tid);
          suma_total = sumaN (suma_total, NUM_SUMADO);
        salidaSeccionCritica(tid);
    
    }
   
    pthread_exit(NULL);

}

/**
* Programa principal. Ejecuta simultáneamente en dos hebras diferentes. E
* Espera a que ambas terminen y mira el valor sumado final, comprobando si es
* el esperado.
*/
int main() {
    
    
   pthread_t th1, th2;
   long resultadoEsperado;
        
    // creamos 2 hilos
    pthread_create(&th1, NULL, run, (void *)0);
    pthread_create(&th2, NULL, run, (void *)1);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    resultadoEsperado = NUM_SUMADO * NUM_VECES * 2;
   
    printf("El resultado final es %ld\n" ,suma_total);
    printf("Esperábamos %ld\n" , resultadoEsperado);

        if (suma_total != resultadoEsperado)
            printf("¡¡¡NO COINCIDEN!!!");

    //thread_exit(NULL);
    exit(0);

} 

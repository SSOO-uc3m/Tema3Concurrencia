/* compilar como gcc -pthread -o main main.c*/
/* taskset 0x01 para ejecutarlo en un único procesador */

#include <stdio.h>  // printf
#include <pthread.h>// pthread_create,pthread_exit
#include <stdlib.h> // exit


#define NUM_THREADS 2
#define true 1
#define false 0
#define NUM_SUMADO 10000
#define NUM_VECES 10000

long suma_total = 0;
int enSeccionCritica[2];
int turno=0;

long sumaN(long acumulador, int n) {
    int i;
    long total = acumulador;

    for (i=0;i<n;i++){
       total += 1;
        
    }
    
    return total;
}

void salidaSeccionCritica(long tid){
    int otraHebra = 1-tid;
    turno = otraHebra;
    enSeccionCritica[tid]=false;
}

void entradaSeccionCritica(long tid){

    int otraHebra = 1-tid;
    enSeccionCritica[tid]=true;

    while (enSeccionCritica[otraHebra]==true){
       if(turno==otraHebra){
            enSeccionCritica[tid]=false;
             while (turno==otraHebra);
            enSeccionCritica[tid]=true;
       }
       else {;
       }
     }//  while
    // Está libre!

}// entradaSeccionCritica

void *run(void *threadid){
    int i;
    long tid = (long)threadid; // keep book of thread's id
    printf("This is thread #%ld!\n", tid);

    for (i=0;i<= NUM_VECES;i++){
     enSeccionCritica[tid]=true;
     
        //start critical section
        entradaSeccionCritica(tid);
        suma_total = sumaN (suma_total, NUM_SUMADO);
         //end critical section
        salidaSeccionCritica(tid);
    
    }
   
    pthread_exit(NULL);

}
int main() {
    
    
   pthread_t threads[NUM_THREADS];

   int rc;
   long t;
   enSeccionCritica[0]=false;
   enSeccionCritica[1]=false;

   for(t=0; t<NUM_THREADS; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, run, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

    for(t=0; t<NUM_THREADS; t++)
        pthread_join(threads[t], NULL);
    

    printf("Suma=%ld\n", suma_total);

    //thread_exit(NULL);
    exit(0);

} 

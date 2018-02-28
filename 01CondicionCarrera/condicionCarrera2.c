/* compilar como gcc -pthread -o main main.c*/

#include <stdio.h>
#include <pthread.h>
#define NUMTH 10

int suma_total = 0;

void suma() {
    int i,n;   
    n=rand()%5;
    for (i=0;i<n;i++){
        printf(".");
        
    }   
   
    suma_total += 100;
}
int main() {
    
    
    pthread_t th[NUMTH];
    int i;
    // creamos 10 hilos
    for (i=0;i<NUMTH;i++) {
        pthread_create(&th[i],NULL,(void*)suma, NULL);
    }

    for (i=0;i<NUMTH;i++) {
         pthread_join(th[i], NULL);
     }
    printf("Suma=%d\n", suma_total);
} 

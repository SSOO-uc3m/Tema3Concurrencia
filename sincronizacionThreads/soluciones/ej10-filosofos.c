/* Este programa resuelve el problema de los filosofos comedores (5 filósofos)
   Utilizando mutex y var condicionales
*/
/* José Manuel Pérez Lobato */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX         5      /* Numero máximo de veces que come cada filosofo*/
#define OCUPADO        1      
#define LIBRE       0      

#define NUMFILOSOFOS 5

pthread_mutex_t mtx;     /* mutex para controlar el acceso a los tenedores*/
pthread_cond_t espera;   /* controla la espera de los filosofos*/
int tenedores[NUMFILOSOFOS];

pthread_mutex_t mtxIndice;     /* mutex para controlar el acceso al indice del filosofo*/  
pthread_cond_t esperaIndice;   /* controla la espera en el índice */
int hiloespera=1;

void  *filosofo(void *indice)  {   /* codigo del que escribe los pares */
   int i,j,tenedor1,tenedor2;
    
   srandom ((unsigned)pthread_self());
 
    pthread_mutex_lock(&mtxIndice);        /* acceder al indice */
       hiloespera=0;
       i=*((int *) indice);
       pthread_cond_signal(&esperaIndice);   
    pthread_mutex_unlock(&mtxIndice);        /* acceder al indice */
      
   tenedor1= i;
   tenedor2= i+1;
   if (tenedor2 == NUMFILOSOFOS) tenedor2=0; 

 for(j=0; j <= MAX; j++ )  {
    pthread_mutex_lock(&mtx);        
      while (tenedores[tenedor1]==OCUPADO || tenedores[tenedor2]==OCUPADO)
            pthread_cond_wait(&espera, &mtx);
      tenedores[tenedor1]=OCUPADO; 
      tenedores[tenedor2]=OCUPADO; 
printf("Filosofo %d va a comer\n",i);
    pthread_mutex_unlock(&mtx); 

    sleep (1+ random()%2);   //cogiendo la comida con los tenedores

printf("Filosofo %d deja de comer\n",i);
    pthread_mutex_lock(&mtx);        
      tenedores[tenedor1]=LIBRE; 
      tenedores[tenedor2]=LIBRE; 
      pthread_cond_broadcast(&espera);   
    pthread_mutex_unlock(&mtx); 
    
    sleep ( random()%3);   //espera un moemnto para masticar
  }
    printf ("FIN filosofo %d\n",i);
    pthread_exit(0);
}


int main(int argc, char *argv[]){
    pthread_t th[NUMFILOSOFOS];
    int i;

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&espera, NULL);
    pthread_mutex_init(&mtxIndice, NULL);
    pthread_cond_init(&esperaIndice, NULL);
   
   for (i=0; i<NUMFILOSOFOS; i++)
     tenedores[i]=LIBRE;

   for (i=0; i<NUMFILOSOFOS; i++){ 
    pthread_mutex_lock(&mtxIndice);        /* acceder al indice */
    pthread_create(&th[i], NULL, filosofo, &i);
      while (hiloespera==1) 
            pthread_cond_wait(&esperaIndice, &mtxIndice ); /* se espera */
      hiloespera=1;
    pthread_mutex_unlock(&mtxIndice);        /* acceder al indice */
   }
   for (i=0; i<NUMFILOSOFOS; i++)
    pthread_join(th[i], NULL);

    pthread_mutex_destroy(&mtx);
    pthread_mutex_destroy(&mtxIndice);
    pthread_cond_destroy(&espera);
    pthread_cond_destroy(&esperaIndice);

    exit(0);
}


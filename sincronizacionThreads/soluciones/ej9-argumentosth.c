/* compile de la siguiente manera: gcc -o argumentosth argumentosth.c -lpthread */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

#define N 4


pthread_cond_t cond;
pthread_mutex_t mutex;
pthread_t thid[N];

int ocupado=1;

void *t0( void *arg){
	int dormir, *id; 
	id = (int*)(arg);
	printf("[th %d] Inicio\n",*id);
	dormir = rand()%3 + 1;
	printf("[th %d] Duerme %d\n",*id,dormir);
	sleep(dormir);
	printf("[th %d] Fin\n",*id);
	pthread_exit(NULL);

}


void *t1( void *arg){
	int dormir, id; 
	id = *((int*)(arg));
	printf("[th %d] Inicio\n",id);
	dormir = rand()%3 + 1;
	printf("[th %d] Duerme %d\n",id,dormir);
	sleep(dormir);
	printf("[th %d] Fin\n",id);
	pthread_exit(NULL);

}


void *t2( void *arg){
	int dormir, id;

	pthread_mutex_lock(&mutex);	
	id = *((int*)(arg));
	pthread_cond_signal(&cond);
	ocupado=0;
	pthread_mutex_unlock(&mutex);

	printf("[th %d] Inicio\n",id);
	dormir = rand()%3 + 1;
	printf("[th %d] Duerme %d\n",id,dormir);
	sleep(dormir);
	printf("[th %d] Fin\n",id);
	pthread_exit(NULL);

}


void *t3( void *arg){
	int dormir, id; 
	id = (int)(arg);
	printf("[th %d] Inicio\n",id);
	dormir = rand()%3 + 1;
	printf("[th %d] Duerme %d\n",id,dormir);
	sleep(dormir);
	printf("[th %d] Fin\n",id);
	pthread_exit(NULL);

}




int main (){
	int i;



	printf("==== [main] Inicio Test 0 Crea N t0 y espera====\n");
	
	for (i=0;i<N;i++)
		pthread_create (&thid[i], NULL, t0, (void *)&i);
	for (i=0;i<N;i++)	
		pthread_join(thid[i], NULL);


	printf("==== [main] Inicio Test 1 Crea N t1 y espera====\n");
	
	for (i=0;i<N;i++)
		pthread_create (&thid[i], NULL, t1, (void *)&i);
	for (i=0;i<N;i++)	
		pthread_join(thid[i], NULL);


	printf("==== [main] Inicio Test 2 Crea N bucles (t1, dormir 1 seg) y espera ====\n");
	
	for (i=0;i<N;i++){
		pthread_create (&thid[i], NULL, t1, (void *)&i);
		sleep(1);
	}
	for (i=0;i<N;i++)	
		pthread_join(thid[i], NULL);

	printf("==== [main] Inicio Test 3 Crea N bucles t2 usando mutex y variables condicion ====\n");
	ocupado = 1;
	pthread_cond_init(&cond, NULL);
        pthread_mutex_init(&mutex, NULL);

	for (i=0;i<N;i++){
		pthread_mutex_lock(&mutex);
		pthread_create (&thid[i], NULL, t2, (void *)&i);
		while(ocupado){
			pthread_cond_wait(&cond,&mutex);
		}
		ocupado=1;
		pthread_mutex_unlock(&mutex);
	}
	for (i=0;i<N;i++)	
		pthread_join(thid[i], NULL);

	printf("==== [main] Fin ====\n");
}


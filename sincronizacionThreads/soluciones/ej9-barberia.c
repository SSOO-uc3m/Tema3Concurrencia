/*Programa que ejecuta el problema del barbero que es un proceso que se queda dormido si no hay clientes en una barberia en la que no caben mas de 4 clientes*/
/*Jose Manuel Perez Lobato*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_CLIENTES 4
#define SEG_LLEGADA_CLIENTE 10

int ocupacion=0;

pthread_mutex_t m;
pthread_cond_t barbero_durmiendo;
pthread_cond_t corte_pelo;


void CortarElPelo() {
  printf("Estoy cortando el pelo....ocupacion=%d\n",ocupacion);
  sleep(3);
  printf("He terminado de cortar el pelo!!!\n");
}

void * barbero ()
{
  while(1)
  {
     pthread_mutex_lock(&m);
     while(ocupacion==0 )
     {
       printf("Soy el barbero y duermo\n");
       pthread_cond_wait(&barbero_durmiendo,&m);
     }
     CortarElPelo();
     ocupacion--;
     pthread_cond_signal(&corte_pelo);
     pthread_mutex_unlock(&m);
  }

  pthread_exit(NULL);
}


void * cliente(void * p) {
  int n_cliente;
  
  n_cliente=(int)p;
  pthread_mutex_lock(&m);
  
  if(ocupacion != MAX_CLIENTES) {
    ocupacion++;
    printf("Soy el cliente %d y acabo de llegar. Ocupacion=%d\n",n_cliente,ocupacion);
    pthread_cond_signal(&barbero_durmiendo);
    pthread_cond_wait(&corte_pelo,&m);
  }
  else
  {
    printf("Soy el cliente %d y no hay sillas. Me voy!!\n", n_cliente);
  }
  pthread_mutex_unlock(&m);      
  pthread_exit(NULL);
}

int main()
{
	int num;
        pthread_t t_barbero;
	pthread_t * p_cliente;
	int contador=0;
		
	pthread_mutex_init(&m,NULL);
	pthread_cond_init(&barbero_durmiendo,NULL);
	pthread_cond_init(&corte_pelo,NULL);

	pthread_create(&t_barbero,NULL,barbero,NULL);

	srand(time(NULL));
	
	while(1) //simulacion de llegada de procesos al sistema
	{
		num=rand()%2;
		if(num==0)
		{
			contador++;
                        p_cliente=malloc(sizeof(pthread_t));
			pthread_create(p_cliente,NULL,cliente,(void*)&contador);
		}
		else
		{
			sleep(2);
		}
	}
	pthread_mutex_destroy(&m);
}

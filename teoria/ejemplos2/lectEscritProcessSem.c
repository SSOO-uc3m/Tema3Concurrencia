#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
	  
key_t dato_key;                 /*      shared memory key       */
int dato_id;                    /*      shared memory id        */
key_t n_lectores_key;                 /*      shared memory key       */
int n_lectores_id;                    /*      shared memory id        */
int *dato ;       /* recurso */
int *n_lectores ; /* numero de lectores */
sem_t *sem_lec;      /* controlar el acceso n_lectores */
sem_t *mutex;        /* controlar el acceso a dato */

void lector(int pid) { /* codigo del lector */
        int i;
  	//for(;;){
	sem_wait(sem_lec);
  	*n_lectores += 1;
  	if (*n_lectores == 1) 
		sem_wait(mutex);
  	sem_post(sem_lec);

  	printf("\tL%d:lee dato %d \n",pid,*dato);  /* leer dato */

  	sem_wait(sem_lec);
  	*n_lectores -= 1;
  	if (*n_lectores == 0) 
		sem_post(mutex);
  	sem_post(sem_lec);
        
	exit(0);	
 
}

void escritor(int pid) {   /* codigo del escritor */
   int i;
   //for(;;){
   	sem_wait(mutex);
   	*dato += 2;     /* modificar el recurso */
   	printf("P%d: modifico dato por %d\n",pid,*dato);
   	sem_post(mutex);
   //}
   
}

int main(int argc, char *argv[]) {

    dato_key = ftok ("/dev/null", 5);  /* valid directory name and a number */
    printf ("dato_key for p = %d\n", dato_key);
    dato_id = shmget (dato_key, sizeof (int), 0644 | IPC_CREAT);
    if (dato_id < 0){                           /* shared memory error check */
	      perror ("shmget\n");
	      exit (1);
    }

    dato = (int *) shmat (dato_id, NULL, 0);   /* attach p to shared memory */
    *dato = 5;

    n_lectores_key = ftok ("/dev/null", 7);  /* valid directory name and a number */
     
    n_lectores_id = shmget (n_lectores_key, sizeof (int), 0644 | IPC_CREAT);
    
    n_lectores = (int *) shmat (n_lectores_id, NULL, 0);   /* attach p to shared memory */
    *n_lectores = 0;
    int  i;
      
     pid_t pid; /* Crea el semáforo nombrado */ 
     if (argc<2){
     	perror("Número de argumentos insuficientes");
	exit(-1);
     }
     
     if((mutex=sem_open("semMutex", O_CREAT| O_EXCL, 0644, 1))==(sem_t *)-1) { 
	     perror("No se puede crear el semaforo"); 
	     exit(-1); 
     }

     if((sem_lec=sem_open("semLect", O_CREAT| O_EXCL, 0644, 1))==(sem_t *)-1) { 
	     perror("No se puede crear el semaforo"); 
	     exit(-1); 
     }
     // Crea los procesos lectores
     
     for (i = 0; i< atoi(argv[1]); ++i){
	pid = fork();
	if (pid ==-1){ 
		perror("No se puede crear el proceso");
		exit(-1);
	}
	if(pid==0) { //child	  	
	  lector(getpid());
	 exit(0);
	  break;
	}
        escritor(pid);	
     }
     
     for(i = 0; i < atoi(argv[1]); ++i){
     	wait(NULL);
     }
     printf("acaban todos\n");
     


     shmdt (n_lectores);
     shmdt(dato);
     shmctl (n_lectores_id, IPC_RMID, 0); 
     shmctl (dato_id, IPC_RMID, 0);
     
     sem_close(mutex);
     sem_close(sem_lec);
     sem_unlink("semMutex");
     sem_unlink("semLect");
     
     exit(0);
     
}


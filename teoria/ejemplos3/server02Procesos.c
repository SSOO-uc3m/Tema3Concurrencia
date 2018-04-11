#include "peticion.h"
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

const int MAX_PETICIONES = 5;
int hijos = 0;

void manejadorHijos (int sig){
  pid_t pid;
  int status;

  while ( waitpid(-1, &status, WNOHANG) > 0){
    hijos--;
    printf("Termina el proceso %d, quedan %d\n",pid,hijos);
  }

}

int main()  {
     int i;
     peticion_t p;
     time_t t1,t2;
     double dif;
     int pid;

     struct sigaction sa;
     sigemptyset(&sa.sa_mask);
     sa.sa_flags = 0;
     sa.sa_handler = manejadorHijos;
     sigaction(SIGCHLD, &sa, NULL);
     t1 = time(NULL);

     for (i=0; i  < MAX_PETICIONES; i++) {
        recibir_peticion(&p);
	/*
	do{
		fprintf(stderr,  "Comprobando hijos \n");
		pid = waitpid(-1, NULL, WNOHANG);
		if (pid>0) {
			hijos--;
		}
	}
	while (pid > 0);
	*/
	pid = fork();
	switch(pid){
	case  -1:
		perror("Error en la creación del hijo");
		break;
	case 0: /* HIJO  */
		responder_peticion(&p);
		exit(0);
		break;
	default:  /* PADRE */
		hijos++;
	}

     }
     fprintf(stderr, "Esperando a que terminen %d hijos\n", hijos);
 
     while (hijos > 0) {
        pause();
/*	pid = waitpid(-1, NULL, WNOHANG); 
	if (pid > 0 ) {
		hijos--;
	}*/
     };

     t2 = time (NULL);

     dif = difftime(t2,t1);
     printf("Tiempo: %lf\n",dif);

    return 0;
 }


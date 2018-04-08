#include "peticion.h"
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

const int MAX_PETICIONES = 5;

int main()  {
     int i;
     peticion_t p;
     time_t t1,t2;
     double dif;
     int pid, hijos = 0;

     t1 = time(NULL);

     for (i=0; i  < MAX_PETICIONES; i++) {
        recibir_peticion(&p);
	do{
		fprintf(stderr,  "Comprobando hijos \n");
		pid = waitpid(-1, NULL, WNOHANG);
		if (pid>0) {
			hijos--;
		}
	}
	while (pid > 0);

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
     fprintf(stderr, "Comprobando %d hijos\n", hijos);
     while (hijos > 0) {
	pid = waitpid(-1, NULL, WNOHANG); 
	if (pid > 0 ) {
		hijos--;
	}
     };

     t2 = time (NULL);

     dif = difftime(t2,t1);
     printf("Tiempo: %lf\n",dif);

    return 0;
 }


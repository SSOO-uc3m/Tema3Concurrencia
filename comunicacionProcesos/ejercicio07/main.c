#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int main( void )
{
  int tuberia[2];
  int pid1, pid2;
  /* el proceso padre, que crea el pipe, será el proceso p1 */
  if (pipe(tuberia) < 0) {
    perror("No se puede crear la tubería") ;
    exit(0);
  }
  /* se crea el proceso p2 */
  switch (pid1=fork()) {
    case -1: perror("Error al crear el proceso") ;
    /* se cierra el pipe */
      close(tuberia[0]) ;
      close(tuberia[1]) ;
      exit(0) ;
      break ;
    case 0: /* proceso hijo, proceso p2 */
    /* se cierra el descriptor de lectura del pipe */
      close(tuberia[0]) ;
      /* aquí iría el código del proceso p2 */
      /* escribiría usando el descriptor tuberia[1] */
      break ;
    default: /* el proceso padre crea ahora el proceso p3 */
      switch (pid2 = fork()) {
        case -1: perror("Error al crear el proceso") ;
          close(tuberia[0]) ;
          close(tuberia[1]) ;
          /* se mata al proceso anterior */
          kill(pid1, SIGKILL) ;
       exit(0) ;
       case 0: /* proceso hijo (p3) lee de la tubería */
          close(tuberia[1]) ;
           /* código del proceso p3 que lee de la tubería */
          break ;
        default: /* el proceso padre (p2) escribe en la tubería */
          close(tuberia[0]) ;
          /* código del proceso p1 que escribe en la tubería */
          break ;
      }
  }
  exit(0);
  
}

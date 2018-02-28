#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
   int fd[2];
   pid_t pid;

   // creamos una tuberia
   if (pipe(fd) < 0)  {
      perror("pipe");
      exit(-1); 
  }
  pid = fork();   
  switch(pid)  {
      case -1:   /* error */
         perror("fork");
         exit(-1);
       case 0:    /* proceso hijo ejecuta ''ls'' */
          close(fd[0]); /* cierra el pipe de lectura */
	  close(STDOUT_FILENO); /* cierra la salida estandar */
	  dup(fd[1]);
	  close(fd[1]);
          execlp("ls","ls",NULL);
          perror("execlp");
          exit(-1);
     default:  /* proceso padre ejecuta ''wc'' */
          close(fd[1]); /* cierra el pipe de escritura */
   	  close(STDIN_FILENO);  /* cierra la entrada estandar */
	  dup(fd[0]);
	  close(fd[0]);
          execlp("wc","wc",NULL);
          perror("execlp");
  }
 return 0;
}

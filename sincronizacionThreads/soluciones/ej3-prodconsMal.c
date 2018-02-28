
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

int n;
semaphore s=1;
semaphore esperar=0;

void productor(void)
{
   while (1)
   {
      producir();
      wait(mutex);
      añadir(buffer); 
      n++;
      if (n==1) signal(esperar);
      signal(mutex);
   }
}

void consumidor(void)
{
   while (1)
   {
      wait(mutex);
      coger(buffer);       
      n--;
      if (n==0) wait(esperar); // peligro de interbloqueo
      signal(mutex);
      consumir();

   }
}



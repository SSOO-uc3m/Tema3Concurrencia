/*Proteger la instrucción que comprueba el valor de n en el consumidor con el semáforo mutex
*/
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
   while (1)   {
      wait(mutex);
      coger(buffer);       
      n--;
      signal(mutex);
      consumir();
     wait(mutex);
      if (n==0) wait(esperar);
      signal(mutex);
   }
}


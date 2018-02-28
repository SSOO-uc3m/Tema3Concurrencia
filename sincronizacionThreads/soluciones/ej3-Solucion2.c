/*Añadir una variable local al procedimiento consumidor y evaluar esta variable para dejarle 
  dormido en lugar de evaluar la variable global n.
*/
int n;
semaphore s=1;
semaphore esperar=0;

void productor(void)
{
   while (1)   {
      producir();
      wait(mutex);
      añadir(buffer); 
      n++;
      if (n==1) signal(esperar);
      signal(mutex);
    }
}

void consumidor(void)
{  int m;       //variable local
   while (1)   {
      wait(mutex);
      coger(buffer);       
      n--;
      m=n;
      signal(mutex);
      consumir();
      if (m==0) wait(esperar);
   }
}

## Ejercicio  1

Escriba un programa que cree 10 threads. Cada uno de ellos calcula el valor del número PI usando el método de Montecarlo y lo almacena  en la posición que le corresponde de en un array. Cuando han terminado todos los threads el programa principal calcula la media de los valores de pi almacenados en el array

Se deben usar mutex y variables condición para que no haya problemas de Carrera.

## Ejercicio 2

El siguiente código implementa una aplicación con dos threads: uno imprime por pantalla los números pares y otro imprime por pantalla los números impares.

```
#include <pthread.h>
#include <stdio.h>

int dato_compartido = 0;


void pares(void)
{  int i;
   for(i=0; i < 100; i++ )
       printf("Thread1 = %d \n", dato_compartido++);
}
void impares(void)
{  int i;
   for(i=0; i < 100; i++ )
       printf("Thread2 = %d \n", dato_compartido++);
}

int main(void)
{
    pthread_t th1, th2;
    pthread_create(&th1, NULL, pares, NULL);
    pthread_create(&th2, NULL, impares, NULL);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
            }

```



Se desea que la ejecución proporcione por pantalla la siguiente salida:

Thread1 = 0

Thread2 = 1

Thread1 = 2

Thread2 = 3

Thread1 = 4

Thread2 = 5

Thread1 = 6

.....

Se ha realizado una primera ejecución del código y aparece lo siguiente:

Thread1 = 0

Thread1 = 1

Thread1 = 2

Thread1 = 3

Thread2 = 3

Thread2 = 4

Thread2 = 5

.....

Se pide resolver los siguientes apartados:

1. Indicar que problemas genera el utilizar una variable compartida para enviar el dato a imprimir desde el **thread pares** al **thread impares**.

1. Implementar una versión del programa anterior que resuelva los problemas anteriores utilizando alguna de las técnicas de gestión de concurrencia.

## Ejercicio 3

Se plantea a los alumnos de Sistemas Operativos de la Universidad Carlos III de Madrid resolver el problema del productor/consumidor con buffer ilimitado (es decir, no existe limitación en el número de elementos que puede generar un productor ya que el buffer de almacenamiento se considera infinito). Se pide a los alumnos que implementen la función productor y la función consumidor usando semáforos, y evitando que se produzcan problemas de concurrencia. Uno de los alumnos entrega la siguiente solución:


```
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
      if (n==0) wait(esperar);
      signal(mutex);
      consumir();

   }
}

```

Dicha solución no es correcta. Se pide:

<ol type="a">
  <li>Encontrar un contraejemplo que suponga el fallo de esta solución.</li>
  <li>Corregir el código de manera que el problema encontrado sea solucionado, o implementar un nuevo código que funcione.</li>
</ol>


**EJERCICIO 4**

Dado el siguiente esquema:

**Proceso P1**              

...                           

accion1()       

**Proceso P2**              

...                           

accion2()          

Asegurar que la acción1() siempre se ejecuta antes que la accion2(), mediante:

<ol type="a">
  <li>Semáforos.</li>
  <li>Mutex y variables condicionales.</li>
</ol>


**EJERCICIO 5**

Lectores-Escritores con semáforos.
<ol type="a">
  <li>Dando prioridad a los lectores _(un escritor no puede acceder a la modificación del recurso si tengo lectores que quieren consultarlo)._</li>
  <li>Dando prioridad a los escritores _(un nuevo lector no puede acceder a la lectura del recurso si existen escritores que desean modificarlo)._</li>
</ol>



**EJERCICIO 6**

Un famoso firma autógrafos en una tienda. El famoso solo puede firmar un autógrafo a la vez. La sala donde se firma tiene un aforo limitado de 20 plazas. El famoso dice que solo saldrá a firmar autógrafos si en la sala hay más de 5 personas. Si no hay al menos 5 personas en la sala, dormirá hasta que las haya (en el momento que haya 4 personas o menos se pondrá a dormir). Las personas que quieran firmar y no puedan entrar a la sala por rebasar el aforo permitido se irán sin poder recibir el autógrafo, las que reciban el autógrafo abandonarán la sala.
El famoso representa a un proceso ligero de un tipo que permanece siempre en el sistema y ejecuta la función _famoso_. Las personas representan a procesos ligeros que ejecutan la función _fan_.

```
void famoso()
{
   while(1)
   {
         //Código del proceso famoso
   }
}
```


```
void fan()
{
   // Código del fan
}

```

Dadas las siguientes definiciones compartidas por todos los procesos:

```
#define AFORO_MAX 20
int ocupacion=0;    //Almacena la ocupación de la sala 
int firmado=0;	//Indica si el famoso ya ha hecho la firma solicitada por el fan
pthread_mutex_t m;  		//Mutex para región crítica
pthread_cond_t famoso_durmiendo; //variable condicional para que el famoso espere dormido hasta que entren 5 personas
pthread_cont_t autografo; 	//variable condicional para que las personas esperen hasta haber recibido su autógrafo
void Firmar();    	//Función a la que debe llamar el famoso para hacer una firma

```

SE PIDE: Codificar las funciones &#39;famoso&#39; y &#39;fan&#39; utilizando el _mutex_ y las _variables condicionales_ dadas.

NOTA: No hay que inicializar los _mutex_ ni las _variables condicionales_, se suponen ya inicializadas.

**EJERCICIO 7**

**Ejercicio de válvulas de riego.**

Realizar un programa en C que sirva para controlar un sistema de riego de 5 válvulas de riego y 3 entradas de agua.

El programa tendrá un thread por cada válvula de riego y un thread por cada entrada.

Se mostrará un menú al usuario que será el que decida si se debe abrir o cerrar una entrada de agua.

Cuando el usuario decide abrir una entrada de agua uno de los threads de entrada se colocará como abierto y cuando decida cerrar una entrada uno de los threads de entrada abiertos pasará a su estado de cerrado

En número de válvulas de riego abiertas debe ser igual o menor que el número de entradas abiertas. Cuando una entrada se abra se debe abrir también una válvula de riego. El funcionamiento de las válvulas de riego debe ser el siguiente;

1. el thread de la válvula debe esperar a que el número de entradas sea mayor que el número de válvulas abiertas,
2. cuando así sea el thread de la válvula intentará tomar el derecho a ser él el que pase a estado de abierto,
3. en este estado estará 3 segundos,
4. después dará paso a otro thread y
5. estará 1 segundo hasta volver a intentar pasar de nuevo al estado de abierto si la situación en ese instante se lo permite.

Durante los 3 segundos de espera con la válvula abierta no será necesario controlar si el número de entradas abiertas es mayor o igual que el número de válvulas.

**EJERCICIO 8**

Indique que realiza el siguiente código:

```
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10
#define TAMANIO 1024

void *trabajador(void *arg);

int vector[TAMANIO];
struct b_s {
  int n;
  pthread_mutex_t m;
  pthread_cond_t ll;
} b;


int main(void) {
  pthread_t hilo[N];
  int i;

  	
  b.n = 0;
  pthread_mutex_init(&b.m, NULL);
  pthread_cond_init(&b.ll, NULL);
  par=0; impar=1;
  
  for(i=0; i<N; i++)
    pthread_create(&hilo[i],
                 NULL, trabajador, 
                 (void *)&i);
  
  for(i=0; i<N; i++)
    pthread_join(hilo[i], NULL);

  pthread_cond_destroy(&b.ll);
  pthread_mutex_destroy(&b.m);

  return 0;
}

void *trabajador(void *arg) {
int inicio=0, fin=0, i;


id  = *(int *)arg; 
inicio =(id)*(TAMANIO/N);

fin = (id+1)*(TAMANIO/N);

for(i=inicio; i<fin; i++) {
	vector[i] = id;
}
pthread_mutex_lock(&b.m); 
b.n++; 
if (N<=b.n) {		pthread_cond_broadcast(&b.ll);
} else {
	pthread_cond_wait(&b.ll, &b.m); 
}
pthread_mutex_unlock(&b.m); 
	
return 0;
}

```



**EJERCICIO 9**

Añadir una variable local al procedimiento consumidor y evaluar esta variable para dejarle dormido en lugar de evaluar la variable global n.

**EJERCICIO 9**

Escribir un programa que ejecuta el problema de la barbería que atiende clientes.  Los  barberos pueden atender como máximo a 4 clientes dentro de la barbería. Si no hay trabajo los barberos duermen.  La barbería se modela como un proceso.  Cada cliente que entra ocupa un sillón. Si ya está todo ocupado, los clientes intentan entrar y si no pueden se van.

**EJERCICIO 10**

Escribir un programa que resuelva el programa de los filósofos que comen, para 5 filósofos usando MUTEX. Cinco [filósofos](https://es.wikipedia.org/wiki/Fil%C3%B3sofo) se sientan alrededor de una mesa y pasan su vida cenando y pensando. Cada filósofo tiene un plato de fideos y un tenedor a la izquierda de su plato. Para comer los fideos son necesarios dos tenedores y cada filósofo sólo puede tomar los que están a su izquierda y derecha.





**EJERCICIO 11**

Realizar un programa que declare una función imprimir y que le pase como parámetros 1 string a imprimir.

A continuación el programa principal debe preparar los parámetros con 2 string &quot;hola&quot; y &quot;mundo \n&quot; y lanzar 2 threads que intenten imprimir &quot;hola mundo&quot; en ese orden N veces y terminar

**EJERCICIO 12**

Escribir un programa sencillo para ver como funcionan los mutex. El programa principal crea 4 threads y espera hasta que han terminado todos. Lo normal sería que el main hiciera un pthread\_join, pero se debe hacer con mutex para que se vea como utilizarlos.

**EJERCICIO 13**

Escribir un programa para probar las barreras de POSIX. El programa debe crear seis threads  y una barrera.  Cada thread debe dormir 3 segundos y esperar para poder pasar la barrera.  El padre debe esperar a que terminen todos los threads.

**EJERCICIO 14**

Implementar un programa que resuelva el problema del productor-consumidor con MUTEX. El programa describe dos thread, productor y consumidor, que comparten un [buffer](https://es.wikipedia.org/wiki/Buffer) de tamaño finito. La tarea del productor es generar un número entero, almacenarlo y comenzar nuevamente; mientras que el consumidor toma (simultáneamente) números uno a uno. El problema consiste en que el productor no añada más números que la capacidad del buffer y que el consumidor no intente tomar un número si el buffer está vacío.

**EJERCICIO 15**

Implementar un programa que resuelva el problema del productor-consumidor con Semáforos POSIX. El programa describe dos thread, productor y consumidor, que comparten un [buffer](https://es.wikipedia.org/wiki/Buffer) de tamaño finito. La tarea del productor es generar un número entero, almacenarlo y comenzar nuevamente; mientras que el consumidor toma (simultáneamente) números uno a uno. El problema consiste en que el productor no añada más números que la capacidad del buffer y que el consumidor no intente tomar un número si el buffer está vacío.

**EJERCICIO 16**

Realizar un programa que cree 10 &quot;threads&quot;, el primer &quot;thread&quot; sumara los números del 001-100 de un fichero que contiene 1000 numeros, y los siguentes &quot;threads&quot; sumaran sucesivamente los numeros que les correspondan: 101-200, 201-300, 301-400, 401-500, 601-700, 701-800, 801-900 y 901-1000 respectivamente. Los hijos devolveran al padre la suma realizada, imprimiendo este la suma total.

Utilice MUTEX para asegurar que no hay problemas de concurrencia entre los threads.

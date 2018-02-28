## Condiciones de carrera

Como se ha visto, el peligro más importante en la programación concurrente es la compartición de recursos; si no se hace con cuidado pueden aparecer condiciones de carrera.

En esta práctica vamos a intentar poner de maniñesto este problema tratando de conseguir que nuestro programa sufra una condición de carrera (y podamos detectarlo). El ejemplo descrito antes, de dos hebras incrementan-
do una variable compartida, puede ocasionar condiciones de carrera, pero es muy improbable (en monoprocesador), y no podemos forzarlo. De hecho, ese es el peor tipo de condiciones de carrera, las improbables, pues ocurren muy
pocas veces, y cuando lo hacen es virtualmente imposible averiguar que ha ocurrido para que el programa haya dejado de funcionar.

Para conseguir una condición de carrera de este tipo (de actualización perdida) lo que vamos a hacer es intentar distanciar en el tiempo el momento en el que se lee el valor de una variable y el momento en el que se modifica
con el nuevo valor tras realizar un cálculo:

```
/* compilar como gcc -pthread -o condicionCarrera.c main.c*/
/* taskset 0x01 para ejecutarlo en un único procesador */

#include <stdio.h>
#include <pthread.h>
#define NUM_SUMADO 10000
#define NUM_VECES 10000

/** 
* Atributo con el valor acumulado donde se realiza la suma.
* Hace las veces de variable compartida entre las dos hebras.
*/
long suma_total = 0;

/**
* Método estático que devuelve acumulador + n. Hace la
* suma de uno en uno con un for.
*/
long sumaN(long acumulador, int n) {
    int i;
    long total = acumulador;
    
    for (i=0;i<n;i++){
       total += 1;
        
    }
    
    return total;
}

/**
* Método todo a ser ejecutado a través de una hebra. Llama
* NUM_VECES a sumaN para sumar NUMERO_SUMADO al atributo _suma.
*/
void run() {
    int i;
    
    for (i=0;i<= NUM_VECES;i++){
        suma_total = sumaN (suma_total, NUM_SUMADO);
    }
   
   
}

    /**
     * Programa principal. Ejecuta simultáneamente en dos hebras diferentes. E
     * Espera a que ambas terminen y mira el valor sumado final, comprobando si es
     * el esperado.
     * 
     */
int main() {
    
    
    pthread_t th1, th2;
    long resultadoEsperado;
        
    // creamos 2 hilos
    pthread_create(&th1,NULL,(void*)run, NULL);
    pthread_create(&th2,NULL,(void*)run, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    resultadoEsperado = NUM_SUMADO * NUM_VECES * 2;
   
    printf("El resultado final es %ld\n" ,suma_total);
    printf("Esperábamos %ld\n" , resultadoEsperado);

        if (suma_total != resultadoEsperado)
            printf("¡¡¡NO COINCIDEN!!!");
        

} 


```

Algunos comentarios del código:

El main lanza dos Threads diferentes. De ese modo, las dos comparten el mismo objeto, que hace las veces de “recurso compartido”.

La única variable global es suma_total. El método run() modifica ese atributo sumándole NUM_VECES veces el valor de NUMERO_SUMADO (ambas constantes).

Como hay dos hebras, y al principio suma_total vale 0, al terminar ambas hebras suma_total deberia valer 2\*NUMERO_SUMADO\*NUM_VECES. En el programa principal esperamos a que las dos hebras terminen, y comprobamos si es así.

En el método run() las sumas se hacen con un for sumando de uno en uno.

Escribe el código y pruebalo. Ejecutalo también usando taskset o start /AFFINITY para comprobar los resultados con un único procesador. El programa deberia quejarse de que el valor
esperado no es igual al obtenido, lo que indica una condición de carrera.

Si no lo consigues, haz más grande la constante NUMERO_SUMADO.

¿Eres capaz de describir por que se produce el problema?

# Exclusión mutua. El algoritmo de Dekker

<!-- MarkdownTOC depth=3 -->

- [Primera versión: Alternancia estricta](#primera-versión-alternancia-estricta)

- [Segunda versión: Problema de interbloqueo](#segunda-versión-problema-de-interbloqueo)

- [Tercera versión: Colisión región crítica no garantiza exclusión mutua](#tercera-versión-colisión-region-crítica-no-garantiza-exclusión-mutua)

- [Cuarta versión: Postergación indefinida](#cuarta-versión-postergación-indefinida)

- [Quinta versión: Solución final](#quinta-versión-solución-final)

<!-- /MarkdownTOC -->

## Primera versión: Alternancia estricta

En el fichero condicionCarrera3.c conseguimos un programa que sufría de condiciones de carrera continuamente) algo que nos permite probar nuestros intentos para evitarla.

La condición de carrera se producía porque el campo `suma_total` era leído primero y modificado un rato después, dando la oportunidad a otra hebra a modificarlo entre medias, y que dicha modificación se perdiera. La solución es hacer uso de exclusión mutua sobre el campo `suma_total` de manera que se garantice que únicamente una hebra esta utilizándola. Es decir, desde que se lee el valor (para pasarlo a la función `sumaN` ) hasta que se establece el nuevo (cuando vuelve) nadie debe acceder al mismo tiempo al valor de `suma_total`. La porción del código fuente durante el que se utiliza un recurso en exclusión mutua se llama _sección critica_. Lo que queremos, por tanto, es que la función run() de condicionCarrera3.c sea algo así:

```
void run() {
    int i;

    for (i=0;i<= NUM_VECES;i++){
        entradaSeccionCritica(tid);
        suma_total = sumaN (suma_total, NUM_SUMADO);
        salidaSeccionCritica(tid);    
    }


}
```


de modo que se garantice que en la sección crítica sólo haya una hebra simultáneamente. El problema de la exclusión mutua es conseguir programar `entradaSeccionCritíca()` y `salidaSeccíonCritica()` sin hacer ningún tipo de suposición sobre el número de procesadores del sistema ni las velocidades relativas de las hebras.

Hay tres tipos de formas de afrontar el problema: por software, por hardware y con la ayuda del sistema Operativo (o del lenguaje). En ésta y las próximas prácticas veremos la primera solución por software conocida como
el **algoritmo de Dekker**.

Este algoritmo es pedagógico porque suele presentarse de manera escalonada, y sirve para demostrar las dificultades que surgen en la programación concurrente. Pero no es útil en la práctica porque) entre otras cosas, exige conocer de antemano las hebras que cooperan y cuál esta entrando (o saliendo) en cada momento de la sección crítica. Esto nos lleva a tener que retocar el método `run()` a:

```
void *run(void *threadid){
    int i;
    long tid = (long)threadid; // id del thread

    for (i=0;i<= NUM_VECES;i++){
     
        entradaSeccionCritica(tid);
        suma_total = sumaN (suma_total, NUM_SUMADO);
        salidaSeccionCritica(tid);
    
    }
   
    pthread_exit(NULL);

}
```
Por su parte el programa principal tendrá que ajustar la creación de las hebras:

```
int main() {
    
    
   pthread_t th1, th2;
   long resultadoEsperado;
        
    // creamos 2 hilos
    pthread_create(&th1, NULL, run, (void *)1);
    pthread_create(&th2, NULL, run, (void *)2);

```

En el primer intento del algoritmo de Dekker se utiliza una variable global (compartida) turno que indica el identificador de la hebra que tiene el turno para entrar en la sección crítica. En `entradaSeccíonCritíca(int)` se
hace una espera activa hasta que el turno sea el mismo que el identificador de nuestra hebra, y en `salídaSeccionCritica(int)` se modifica la variable del turno para cederlo a la hebra siguiente:

```

void salidaSeccionCritica(long numHebra){
    int otraHebra = 1-numHebra;
    turno = otraHebra;
}

void entradaSeccionCritica(long numHebra){
    //pthread_yield();
    while(turno !=numHebra);
    // Nos toca!

}// entradaSeccionCritica

```

* Haz una copia del programa condicioncarrera3.c y renombralo como dekker1.

* Modifica el método `run()` y el método `main()` como se ha descrito arriba.

* Añade las nuevas funciones y variables globales .

* Ejecuta el programa varias veces (en multinúcleo) y comprueba que la condición de carrera ha desaparecido.

* Ejecuta el programa en un solo núcleo (con taskset o start / affinity). Comprueba la diferencia en velocidad. ¿A que es debido?

* La variable `turno` la hemos declarado `volatile`. Quita este modificador y vuelve a probar la práctica. ¿Notas alguna diferencia?

* Para paliar la penalización de la espera activa, añade `sched_yield()` en el interior del bucle del método `entradaSeccionCritica()`.

* Esta primera solución software a la exclusión mutua funciona. ¿Qué problemas le ves?

La primera versión de este algoritmo tiene las siguientes catacterísitcas

* Garantiza la exclusión mutua
* Su sincronización es forzada
* Acopla fuertemente a los procesos (procesos lentos atrasan a procesos rápidos)
* No garantiza la progresión, ya que si un proceso por alguna razón es bloqueado dentro o fuera de la sección puede bloquear a los otros procesos

## Segunda versión: Problema de interbloqueo

La primera versión sufría de inanición en ausencia de contención) pues una hebra podría estar esperando su turno para entrar en la sección crítica incluso aunque la otra no esté dentro.

La siguiente propuesta es no guardar el `turno`, sino si una hebra ha entrado en la sección crítica o no. Antes de entrar una hebra mira si la opuesta esta ya dentro. Si lo está, espera hasta que salga, y entra cuando lo haga.

En lugar de un único entero turno, ahora necesitamos un booleana para cada una de las dos hebras.

* Copia fichero de la práctica anterior, y renombra el fichero a dekker2.c

* Modifica el código relacionado con la exclusión mutua:

```
typedef struct
{
    volatile int valor;

} flag;

flag enSeccionCritica[2];

void entradaSeccionCritica(long numHebra){
    int otraHebra = 1-numHebra;

    while(enSeccionCritica[otraHebra].valor==true)
    pthread_yield();

    // ¡Está libre! 
    enSeccionCritica[numHebra].valor = true;
}// entradaSeccionCritica

void salidaSeccionCritica(long numHebra){

     enSeccionCritica[numHebra].valor = false;

}// salidaSeccionCritica
 

```

* Inicializa en el main `enSeccionCritica`. Hemos necesitado crear la estructura `flag` porque un array volatil hace volátil la referencia al array, no los elementos que contiene, que es lo que nosotros necesitamos.
* Compiar y ejecuta el programa. `[gcc -pthread -Wall dekker3.c -o main; &main` ¿Funciona?
* Ejecutalo en monoprocesador (con `taskset `en linux). ¿Funciona ahora?
* ¿Eres capaz de explicar lo que ocurre?

Conclusión:

* Garantiza la exclusión mutua
* No garantiza espera limitada

## Tercera versión: Colisión región crítica no garantiza exclusión mutua

El intento de la práctica anterior no garantizaba la exclusión mutua. En `entradaSeccionCritíca(int)` hay un punto débil en la línea 6 del código, justo cuando salimos del bucle de la espera activa y antes de avisar de que
vamos a entrar en la sección crítica nosotros. La otra hebra podría estar en ese momento comprobando precisamente lo mismo, viendo que nosotros no estamos dentro, y entrando ella también.

La solución intuitiva es que una hebra primero active su bandera, y luego haga espera activa sobre la bandera de la otra hebra. De esa forma solucionamos la exclusión mutua porque en cuanto salimos de la espera activa la
sección crítica es para nosotros al haberla “bloqueado” previamente.

Haz una copia del proyecto de fichero anterior, y renombralo como fichero a dekker3.c

* Modifica la función `entradaSeccionCrítica(long numHebra)`:

```
void entradaSeccionCritica(long numHebra){
     //Simulamos que estamos dentro
     enSeccionCritica[numHebra].valor==true;
     
     int otraHebra = 1-numHebra;

    while(enSeccionCritica[otraHebra].valor==true)
        pthread_yield();

    // ¡Está libre! 
    enSeccionCritica[numHebra].valor = true;
}// entradaSeccionCritica

```

* Ejecuta el programa ¿Funciona?
* Ejecutalo en monoprocesador (con `taskset ` en Linux o start /affinity 0x1 en Windows ). ¿Funciona ahora?
* Eres capaz de explicar lo que ocurre?

Conclusión:

* No garantiza la exclusión mutua
* Colisión en la región crítica

## Cuarta versión: Postergación indefinida 

La solución anterior no funciona porque sufre interbloqueo. El punto delicado en este caso está al inicio de la funcin `entradaSeccionCritica(long numHebra)`. Si las dos hebras, simultáneamente ponen a cierto sus banderas, luego quedarán
indefinidamente esperando a que la otra salga de la sección crítica. En monoprocesador es difícil que el azar genere interbloqueo, pero puedes forzarlo añadiendo un `pthread_yield()` .

```
//Simulamos que estamos dentro
 enSeccionCritica[numHebra].valor==true;
 pthread_yield();
```

El problema principal se debe a que la semántica de las banderas ahora no es si una hebra ha entrado en la sección crítica, sino si tiene intención de entrar, dado que las estamos activando antes de hacerlo. El interbloqueo se produce porque las dos hebras ven que la otra tiene intención de entrar, asumen que, de hecho, ya ha entrado, y esperan a que salga.

La solución es hacer que las hebras tengan paciencia y anulen, temporalmente, su intención de entrar en la sección crítica si ven que la otra hebra también lo ha hecho. De esa forma se evita el interbloqueo, porque el recurso
“se libera”, se espera un instante, y se vuelve a intentar.

Haz una copia del proyecto de la práctica anterior, y renombra la clase a dekker4.c.


* Modifca la función `entradaSeccionCritica(long numHebra)`:

```
void entradaSeccionCritica(long numHebra) {

	//Simulamos que estamos dentro
	enSeccionCritica[numHebra].valor==true;

	int otraHebra = 1-numHebra;
	while(enSeccionCritica[otraHebra].valor==true){
		// Dejamos pasar al otro...
		enSeccionCritica[numHebra].valor==false;
		// ... esperamos un momento...
		pthread_yield();
		// ... y volvemos a intentarlo.
		enSeccionCritica[numHebra].valor==true;
	}
	
	// ¡Está libre!
} // entradaSeccionCritica
```
* Ejecuta el programa.  ¿Funciona?

* Ejecutalo en monoprocesador (con `taskset ` en Linux o start /affinity 0x1 en Windows ) ¿Funciona ahora?

* Dónde está el problema?

Conclusión:

* Garantiza la exclusión mutua.
* Un proceso o varios se quedan esperando a que suceda un evento que tal vez nunca suceda (espera activa).

## Quinta versión: solución final

La solución de la práctica anterior sufre livelock (“bloqueo vital”) si las dos hebras insisten en ceder el turno a la otra, entrando en un bucle de “cortesía mutua”. Es poco probable que esta amabilidad dure para siempre
(porque el planificador no mantendrá “sincronizadas” las dos hebras tanto tiempo), pero aun así no es una alternativa válida.

La solución es mezclar esta última propuesta con la primera en la que llevábamos el turno. Además de tener las dos banderas, llevamos quién tiene el turno para los casos de empate. Si una hebra muestra su intención de entrar en la sección crítica, y al comprobar las intenciones de la otra hebra ve que su bandera también está activa, entonces cambiará momentáneamente de opinión sólo si no es su turno. Si es su turno, entonces asumirá que la otra la dejará pasar a ella, e insistirá en esperar a que desista. De otro modo, desistirá ella, y volverá a probar un momento después.

Haz una copia del fichero de la práctica anterior, y renombra la clase a dekkerFinal.c
Modifica la función `entradaSeccionCritica(long numHebra)`:

```
void entradaSeccionCritica(long numHebra) {
	//Simulamos que estamos dentro
	 enSeccionCritica[numHebra].valor==true;

 	int otraHebra = 1-numHebra;
 	while(enSeccionCritica[otraHebra].valor==true){
 		if (turno == otraHebra) {	
 			// Dejamos pasar al otro...
			enSeccionCritica[numHebra].valor==false;
			while(turno == otraHebra);// Espera activa		
			enSeccionCritica[numHebra].valor==true;
		}
		
	// ¡Está libre!

} // entradaSeccionCritica

```
* Ejecuta el programa y comprueba que funciona.
* Encuentras algún problema a este algoritmo?

Conclusión:

* Garantiza la exclusión mutua.
* Progreso
* Espera limitada

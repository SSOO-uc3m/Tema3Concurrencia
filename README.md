# Tema 3 Concurrencia 

> 
There is always a well-known solution to
every human problem - ueat, plausible,
and wrong.
H.L. Mencken
> 

RESUMEN: En este tema veremos los problemas que las hebras pueden sufrir a la hora de compartir recursos, y varias de alternativas clásicas para (intentar) resolverlos. Se pone de manifiesto la dificultad de la programación concurrente.

Disponer de la posibilidad de crear hebras es el primer paso para conseguir concurrencia. Merece la pena analizar con un poco de cuidado los principales problemas de la programación concurrente. Cuando dos hebras (o dos procesos) colaboran entre sí, lo normal es que, al menos, compartan recursos, como variables comunes, archivos o bases de datos. Las hebras accederán a esos recursos para consultarlos o para modificarlos, y deberán ser conscientes de que, al mismo tiempo, hay otros que están haciendo lo mismo.

Un ejemplo clásico de recurso es “un dato” (ya sea una variable, una base de datos o un fichero en disco) que algunas hebras querrán leer, y otras querrán escribir. En un determinado momento puede haber muchas hebras que lean el dato sin que se produzcan problemas; sin embargo la escritura debe realizarse con más cuidado, porque mientras una hebra escribe ninguna otra debería leer ni escribir el mismo dato.

En la programación secuencial habitual esto no es un problema, pero en la programación concurrente sí. Si tenemos varias hebras que quieren acceder al mismo recurso en un sistema multiprocesador, necesitaremos algún tipo de sincronización. Lo mismo ocurre si estamos en un sistema monoprocesador en el que el uso del recurso puede exceder el cuanto de tiempo concedido a la ejecución de cada hebra.

En general, existen los siguientes problemas:

* **Necesidad de exclusión mutua**: se debe garantizar que las hebras utilizan los recursos compartidos de una en una, y no simultáneamente. Si una hebra esta usando un determinado recurso y otra lo necesita, tendrá que esperar. Las porciones de código en los que una hebra hace uso de un recurso que debe ser utilizado en exclusión mutua se conocen como secciones criticas.

* **Interbloqueos**: la necesidad de exclusión mutua puede ocasionar interbloqueos si dos hebras necesitan, simultáneamente, más de un recurso. Cada hebra podía reservar uno de los recursos y quedar esperando indefinidamente a que el otro quede libre.

* **Inanición** una hebra no debería esperar indefinidamente a que se le conceda el uso de un recurso. Hay dos variantes:

1. Inanición en presencia de contención: un recurso muy solicitado es concedido una y otra vez a una o varias de las hebras solicitantes, y no es entregada nunca a otra. Es la más conocida.

2. Inanición en ausencia de contención: un recurso está libre, y aun así una hebra que desea utilizarlo no recibe el permiso para hacerlo.

* **Coherencia de los datos**: si _varios recursos_ deben guardar entre si algún tipo de relación (por ejemplo, una variable debe tener siempre el doble del valor de otra), se debe garantizar que las operaciones sobre ellas sean “atómicas” desde el punto de vista del resto de las hebras para no sufrir actualizaciones perdidas.

Todos estos problemas de la programación concurrente indican que se debe tener mucho cuidado al hacer uso de recursos compartidos. De otro modo se podrían sufrir condiciones de carrera (en inglés race condition).
Se produce una condición de carrera entre dos procesos o hebras cuando el resultado final depende del orden en el que se ejecuten. En un programa secuencial, con un orden total, esto no es un problema; pero las instrucciones de los programas concurrentes tienen un orden parcial, y si el resultado de la ejecución depende del orden (arbitrario) escogido por el sistema Operativo entonces el programa no será correcto.

Un ejemplo muy simple de condición de carrera es la compartición de una variable global, n, en dos hebras. Si ambas ejecutan:

 `++n;`

ambas tendrán que leer el valor de la variable de memoria, hacer la suma, y volcar el resultado. Si la ejecución de las lecturas se entrelaza, una de las actualizaciones se perderá, algo que no ocurrirá si no se entrelazan.

La necesidad de exclusión mutua, y el riesgo de interbloqueos o inanición lo sufren no sólo hebras que cooperan entre sí, sino también procesos independientes qne compiten en el uso de recursos. Un ejemplo clásico es el uso de
una impresora por parte de dos procesos diferentes e independientes. Entre ellos no se conocen, pero ambos compiten, sin saberlo, por un recurso que debe usarse en exclusión mutua. Es el sistema operativo quién debe lidiar con la asignación de recursos, pero esto demuestra que, efectivamente, los sistemas operativos son aplicaciones concurrentes que deben tener cuidado de los mismos problemas que cualquier otra (y muchos más).

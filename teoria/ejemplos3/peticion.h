#ifndef PETICION_H
#define PETICION_H

#include "unistd.h" // sleep
#include "stdio.h" // fprintf
#include "stdlib.h" // rand, sleep

struct peticion {
   long id;
   /* Resto de campos necesarios */
   int tipo;
   char url[80];
   /* ... */
};

typedef struct peticion peticion_t;

void recibir_peticion (peticion_t * p);
void responder_peticion  (peticion_t * p);

#endif

static long petid = 0;

void recibir_peticion (peticion_t * p)
{
   int delay;
   fprintf(stderr, "Recibiendo petición\n");
   p->id = petid++;

   /* Simulación de tiempo de E/S */
   delay = rand() % 5;
   sleep(delay);

   fprintf(stderr,"Petición %d recibida después de %d segundos\n",
         p->id, delay);
}

void responder_peticion (peticion_t * p)
{
  int delay, i;
  double x;
  fprintf(stderr, "Enviando petición %d\n", p->id);

  /* Simulación de tiempo de procesamiento */
  for (i=0;i<1000000;i++) { x = 2.0 * i; }

  /* Simulación de tiempo de E/S */
  delay = rand() % 20;
  sleep(delay);

  fprintf(stderr, "Petición %d enviada después de %d segundos\n",
       p->id, delay);
}


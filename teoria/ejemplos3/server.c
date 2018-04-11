#include "peticion.h"
#include <time.h>

const int MAX_PETICIONES = 5;

int main()  {
     int i;

     peticion_t p;
     time_t t1,t2;
     double dif;
     t1 = time(NULL);

     for (i=0; i  < MAX_PETICIONES; i++) {
        recibir_peticion(&p);
        responder_peticion(&p);
     }
     t2 = time (NULL);

     dif = difftime(t2,t1);
     printf("Tiempo: %lf\n",dif);

    return 0;
 }


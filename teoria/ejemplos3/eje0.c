#include "peticion.h"

  int main()  {
     peticion_t p;

     for (;;) {
        recibir_peticion(&p);
        responder_peticion(&p);
     }
    
    return 0;
 }


#include <stdio.h>
#include <stdlib.h>
#include "serverlib.h"

int main()
{
    tArbol indiceUsuario, indiceRanked;
    if(chequearEstado() == 0){
        ///seReProcesaTodo();
        printf("Se recuperaron los indices afectados por el cierre inesperado...\n\n");
    }
    runServer(&indiceUsuario, &indiceRanked);
    return 0;
}

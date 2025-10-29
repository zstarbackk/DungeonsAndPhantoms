#include <stdio.h>
#include <stdlib.h>
#include "serverlib.h"

int main()
{

    if(chequearEstado() == 0){
        ///seReProcesaTodo();
        printf("Se recuperaron los indices afectados por el cierre inesperado...\n\n");
    }
    runServer();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "serverlib.h"

int main()
{
    ///comentar si se desea perdudar resultados
    //crearArchivos();

    if(chequearEstado() == 0){
        crearArchivoIndice("usuarios.dat","usuariosNombre.idx",sizeof(tIndiceUsuarioNombre), leerDatosArchivoUsuarioParaIdx, compararIndUsu);
        crearArchivoIndice("partidas.dat","partidas.idx", sizeof(tIndicePartida), leerDatosArchivoPartida, compararIndPar);
        printf("Se recuperaron los indices afectados por el cierre inesperado...\n\n");
    }
    runServer();
    return 0;
}

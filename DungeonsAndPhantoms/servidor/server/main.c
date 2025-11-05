#include <stdio.h>
#include <stdlib.h>
#include "serverlib.h"

int main()
{
    ///Comentar si ya existen los archivos y se desea perdudar los resultados que se agregan
    crearArchivos();

    if(chequearEstado() == 0){
        crearArchivoIndice("usuarios.dat","usuariosNombre.idx",sizeof(tIndiceUsuarioNombre), leerDatosArchivoUsuarioParaIdx, compararIndUsu);
        crearArchivoIndice("partidas.dat","partidas.idx", sizeof(tIndicePartida), leerDatosArchivoPartida, compararIndPar);
        printf("Se recuperaron los indices afectados por el cierre inesperado...\n\n");
    }
    runServer();
    return 0;
}

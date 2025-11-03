#include "arbol.h"
#include "funciones.h"

int main()
{
    int altura, i;
    tPersona dato;
    FILE *pf;
    tArbol arbol;

    crearLote();
    crearArbolB(&arbol);
    crearArchivoIndice("datos.dat", "datos.idx", sizeof(tIndicePersona), leerDatosArchivoPer, compararIndPer);
    pf = fopen("datos.idx", "r+b");
    if(!pf) return 1;
    cargarArbolDesdeArchivoOrdenado(&arbol, pf, sizeof(tIndicePersona), leerDatosArchivoIdx);
    fclose(pf);

    altura = alturaArbol(&arbol);
    for(i=0;i<altura;i++){
        verNodosDeNivel(&arbol, i, mostrarPersonaIdx);
        SALTO
    }

    pf = fopen("datos.dat", "r+b");
    if(!pf) return 1;
    dato.dni = 11;
    buscarEnArbolIndice(&arbol, &dato, &dato.dni, pf, leerDatosArchivoPerConIdx, compararClaveConIndPer);
    SALTO
    printf("Nombre: %s\n Dni: %ld\n Sexo: %c", dato.nya, dato.dni, dato.sex);
    return 0;
}

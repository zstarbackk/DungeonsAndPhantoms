#include "funciones.h"

int leerDatosArchivoUsuarioConIdx(void* dest, FILE *arch, void *param){
    tIndiceUsuarioNombre *idx = (tIndiceUsuarioNombre*)param;
    fseek(arch, idx->offset, SEEK_SET);
    return fread(dest, sizeof(tUsuario), 1, arch);
}
int leerDatosArchivoUsuarioParaIdx(void* dest, FILE *arch, void *param){
    tUsuario usuario;
    tIndiceUsuarioNombre ind;
    ind.offset = ftell(arch);

    if(fread(&usuario, sizeof(tUsuario), 1, arch)){
        ind.id = usuario.id;
        strcpy(ind.usuario, usuario.usuario);
        memcpy(dest, &ind, sizeof(tIndiceUsuarioNombre));
        return sizeof(tIndiceUsuarioNombre);
    }
    else{
        return 0;
    }
}
int leerDatosArchivoPartida(void* dest, FILE *arch, void *param){
    tPartida partida;
    tIndicePartida ind;
    ind.offset = ftell(arch);

    if(fread(&partida, sizeof(tPartida), 1, arch)){
        ind.id=partida.id;
        ind.puntaje = partida.puntaje;
        memcpy(dest, &ind, sizeof(tIndicePartida));
        return sizeof(tIndicePartida);
    }
    else{
        return 0;
    }
}
int compararIndPar(const void*d1, const void *d2){
    tIndicePartida * par1 = (tIndicePartida*)d1;
    tIndicePartida * par2 = (tIndicePartida*)d2;
    int aux;
    aux = par1->puntaje-par2->puntaje;
    if(aux!=0)
        return aux;
    return par1->id -par2->id;
}
int compararIndUsu(const void*d1, const void *d2){
    tIndiceUsuarioNombre * usr1 = (tIndiceUsuarioNombre*)d1;
    tIndiceUsuarioNombre * usr2 = (tIndiceUsuarioNombre*)d2;
    return strcmp(usr1->usuario, usr2->usuario);
}
int compararIndUsuClave(const void*d1, const void *d2){
    char *nombre = (char*)d1;
    tIndiceUsuarioNombre *usr = (tIndiceUsuarioNombre*)d2;
    return strcmp(nombre, usr->usuario);
}
void crearArchivos(){
    FILE *fUsuarios, *fPartida;
    tUsuario usuarios[] = {
        {1,"zstarback","contrasenia1",230,1},
        {2,"nabulecho","contrasenia2",500,1},
        {3,"diegxm","contrasenia4",5250,3},
        {4,"montapuercos","contrasenia5",0,0},
        {5,"bananirou","contrasenia6",1000,1}
    };
    tPartida partida[] ={
        {1,"zstarnack",230,20},
        {2,"nabulecho",500,10},
        {3,"diegxm",5000,400},
        {4,"bananirou",1000,40},
        {5,"diegxm",200,10},
        {6,"diegxm",50,4}
    };
    fUsuarios = fopen("usuarios.dat","wb");
    if(!fUsuarios)
        return;
    fPartida = fopen("partidas.dat","wb");
    if(!fPartida) {
        fclose(fUsuarios);
        return;
    }

    fwrite(partida, sizeof(partida),1,fPartida);
    fwrite(usuarios,sizeof(usuarios),1,fUsuarios);

    ///Si se vuelve a crear el lote de prueba se deben eliminar los viejos indices
    remove("partidas.idx");
    remove("usuariosNombre.idx");
    remove("state.txt");

    fclose(fUsuarios);
    fclose(fPartida);
}

int buscarUsuario(tArbol *p, FILE *fUsuarios, char * nombre, char *contrasenia){
    tUsuario usr;
    int res;
    res = buscarEnArbolIndice(p, &usr, nombre, fUsuarios, leerDatosArchivoUsuarioConIdx, compararIndUsuClave);
    if(res == 0)
        return -1;
    if(strcmp(usr.contrasenia, contrasenia) != 0)
        return 0;
    return 1;
}
int darDeAlta(char * usuario, char * contrasenia, tArbol *pArbolIdxUsu, FILE *pf){
    tUsuario user, aux;
    tIndiceUsuarioNombre idx;
    ///Lo agrego al archivo
    strcpy(user.usuario, usuario);
    strcpy(user.contrasenia, contrasenia);
    user.puntosTotales = 0;
    user.cantPartidas = 0;
    fseek(pf, -(long int)sizeof(tUsuario), SEEK_END);
    ///id autoincremental
    if(fread(&aux, sizeof(tUsuario), 1, pf)){
       user.id = aux.id + 1;
    }
    else{
        user.id = 0;
    }
    fseek(pf, 0L, SEEK_END);
    fwrite(&user, sizeof(tUsuario), 1, pf);
    ///Lo agrego al arbol
    idx.id = user.id;
    idx.offset = ftell(pf) - sizeof(tUsuario);
    strcpy(idx.usuario, user.usuario);
    insertarEnArbolBRec(pArbolIdxUsu, &idx, sizeof(tIndiceUsuarioNombre), compararIndUsu);
    return 1;
}
void mostrarUsuario(void* el){
    tIndiceUsuarioNombre *usr = (tIndiceUsuarioNombre*)el;
    printf("%s\n", usr->usuario);
}
void mostrarRanked(void* el){
    tIndicePartida *part = (tIndicePartida*)el;
    printf("%d\n", part->puntaje);
}

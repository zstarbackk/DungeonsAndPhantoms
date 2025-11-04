#include "funciones.h"

int leerDatosArchivoUsuario(void* dest, FILE *arch, void *param){
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
int compararIndPer(const void*d1, const void *d2){
    tIndiceUsuarioNombre * usr1 = (tIndiceUsuarioNombre*)d1;
    tIndiceUsuarioNombre * usr2 = (tIndiceUsuarioNombre*)d2;
    return strcmp(usr1->usuario, usr2->usuario);
}
int leerDatosIdxUsuario(void* dest, FILE *arch, void *param){
    int med = *(int*)param;

    fseek(arch, med * sizeof(tIndiceUsuarioNombre), SEEK_SET);

    if(fread(dest, sizeof(tIndiceUsuarioNombre), 1, arch))
        return sizeof(tIndiceUsuarioNombre);
    else
        return 0;
}
int leerDatosIdxPartida(void* dest, FILE *arch, void *param){
    int med = *(int*)param;

    fseek(arch, med * sizeof(tIndicePartida), SEEK_SET);

    if(fread(dest, sizeof(tIndicePartida), 1, arch))
        return sizeof(tIndicePartida);
    else
        return 0;
}
void crearArchivos(){
    tUsuario usuarios[] = {
        {1,"zstarback","qcyo123",230,1},
        {2,"nabulecho","vllc",0,0},
        {3,"diegxm","pollera",500,1},
        {4,"tiagopijita","qcyo123",0,0},
        {5,"bananirou","digodigo",5000,1}
    };
    tPartida partida[] ={
        {1,"zstarnack",230,20},
        {2,"nabulecho",500,10},
        {3,"diegxm",5000,400}
    };
    FILE * fUsuarios = fopen("usuarios.dat","wb");
    if(fUsuarios==NULL)
        return;
    FILE * fPartida = fopen("partida.dat","wb");
    if(fPartida==NULL) {
        fclose(fUsuarios);
        return;
    }

    fwrite(partida, sizeof(partida),1,fPartida);

    fwrite(usuarios,sizeof(usuarios),1,fUsuarios);

    fclose(fUsuarios);
    fclose(fPartida);
}
int buscarUsuario(char * nombre, char * text){
    return 1;
}
int darDeAlta(char * usuario, char * contrasenia){
    tUsuario user;
    FILE * pf = fopen("usuarios.dat", "ab");
    if(pf==NULL){
        return 0;
    }
    strcpy(user.usuario, usuario);
    strcpy(user.contrasenia, contrasenia);
    user.puntosTotales = 0;
    user.cantPartidas = 0;
    fwrite(&user, sizeof(tUsuario), 1, pf);
    fclose(pf);
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

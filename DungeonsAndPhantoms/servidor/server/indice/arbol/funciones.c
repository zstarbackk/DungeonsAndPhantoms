#include "funciones.h"


void crearLote(){
    tPersona personas[] = { {45, "nahuel", 'm'},
                            {12, "maria", 'f'},
                            {98, "fernando", 'm'},
                            {11,"Alejandro",'m'},
                            {54,"Miriam",'f'},
                            {85,"Alejandra",'f'},
                            {23,"Paredes",'m'},
                            {99,"Lionel",'m'},
                            {53,"Emiliano",'m'},
                            {76,"Morena",'f'},
                            {51,"Jorge",'m'},
                            {34,"Marchesin",'m'},
                            {10,"Mauricio",'m'},
                            {91,"Cristina",'f'},
                            {44,"Almiron",'m'},
                            {22,"Julia",'f'},
                            {35,"Juliana",'f'},
                            {74,"Malena",'f'},
                            {13,"Joel",'m'},
                            {23,"Mauro",'m'},
                            {66,"Martina",'f'},
                            {15,"Nestor",'m'},
                            {93,"Gaby",'f'},
                            {17,"cande",'f'},
                            {95,"mateo",'m'},
                            };
    FILE *pf = fopen("datos.dat", "wb");
    if(!pf) return;
    fwrite(personas, sizeof(personas), 1, pf);
    fclose(pf);
}

int compararInt(const void*d1, const void*d2){
    return (*(int*)d1 - *(int*)d2);
}
void mostrarInt(void *d){
    printf("[%d]", *(int*)d);
}

int leerDatosArchivoPer(void* dest, FILE *arch, void *param){
    tPersona per;
    tIndicePersona ind;
    ind.offset = ftell(arch);

    if(fread(&per, sizeof(tPersona), 1, arch)){
        ind.dni = per.dni;
        memcpy(dest, &ind, sizeof(tIndicePersona));
        return sizeof(tIndicePersona);
    }
    else{
        return 0;
    }
}
int compararIndPer(const void*d1, const void*d2){
    tIndicePersona *per1 = (tIndicePersona*)d1, *per2 = (tIndicePersona*)d2;
    return (per1->dni - per2->dni);
}
int compararClaveConIndPer(const void *clave, const void *ind){
    tIndicePersona *per = (tIndicePersona*)ind;
    return (*(int*)clave - per->dni);
}
int leerDatosArchivoIdx(void* dest, FILE *arch, void *param){
    int med = *(int*)param;

    fseek(arch, med * sizeof(tIndicePersona), SEEK_SET);

    if(fread(dest, sizeof(tIndicePersona), 1, arch))
        return sizeof(tIndicePersona);
    else
        return 0;
}

void mostrarPersonaIdx(void *d){
    tIndicePersona *per = (tIndicePersona*)d;
    printf("[%ld]", per->dni);
}
int leerDatosArchivoPerConIdx(void* dest, FILE *arch, void *param){
    tIndicePersona *pIdx = (tIndicePersona*)param;
    fseek(arch, pIdx->offset, SEEK_SET);
    return fread(dest, sizeof(tPersona), 1, arch);
}









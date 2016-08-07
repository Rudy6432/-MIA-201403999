#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//ESTRUCTURAS UTILES PARA EL ALMACENAMIENTO DE DATOS

typedef struct PARTITION
{
    char status[10];
    char tipo[10];
    char ajuste[10];
    int comienzo;
    int tamanio;
    char nombre[26];
} PARTITION;

typedef struct MBR
{
    int tamanio;
    char fecha_creacion [128];
    int disk_signature;
    PARTITION particion1;
    PARTITION particion2;
    PARTITION particion3;
    PARTITION particion4;
} MBR;

typedef struct EBR
{
    char status[10];
    char ajuste[12];
    int comienzo;
    int tamanio;
    int siguiente;
    char nombre[26];
} EBR;

#define sizeof_mbr sizeof(MBR)
#define sizeof_ebr sizeof(EBR)
#define sizeof_partition sizeof(PARTITION)


//////////////////PARTE PARA FUNCION MKDISK////////////////// 1era Funcion
int mkdisk(int size, char unit, char *path, char *name)
{

    //codigo para crear la carpeta si no existe
    struct auxiliar;
    struct stat st = {0};

    if (stat(path, &st) == -1)
    {
        char *command="mkdir \"";
        char *comando = (char *) malloc(1 + strlen(command)+ strlen(path));
        strcpy(comando, command);
        strcat(comando, path);
        strcat(comando, "\"");
        printf("%s\n", comando);
        system(comando);
    }

    //codigo para crear el archivo con el tamaño indicado
    char comandoArchivo[255];
    char fullpath [200];
    sprintf(fullpath,"%s%s", path, name);
    int tamanio;
    if(unit=='m' || unit=='\0')
    {
        tamanio = size*1024*1024;
    }
    else if(unit=='k')
    {
        tamanio = size*1024;
    }

    //se verifica si el archivo ya existe
    if (stat(fullpath, &st) != -1)
    {
        printf("Error al crear disco: archivo previamente creado\n");
        return 1;
    }


    //momento en el que realmente se crea el archivo conforme al comando
    char buffer[1024];
    int i=0;


    FILE *disco = fopen(fullpath,"wb+");
    for(i=0; i<(tamanio/1024); i++)
    {
        fwrite(&buffer,1024,1,disco);
    }

    fclose(disco);


    //Para almacenar el MBR al comienzo del archivo
    MBR mbr;
    //parte para el tamaño
    mbr.tamanio=tamanio;
    //parte para la fecha
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
    strcpy(mbr.fecha_creacion,output);
    //parte para el numero identificador aleatorio
    srand (time(NULL));
    int numero = rand();
    mbr.disk_signature=numero;
    mbr.particion1.comienzo=0;
    mbr.particion1.tamanio=0;
    strcpy(mbr.particion1.status,"i");
    strcpy(mbr.particion1.nombre,"");
    mbr.particion2.comienzo=0;
    mbr.particion2.tamanio=0;
    strcpy(mbr.particion2.status,"i");
    strcpy(mbr.particion2.nombre,"");
    mbr.particion3.comienzo=0;
    mbr.particion3.tamanio=0;
    strcpy(mbr.particion3.nombre,"");
    strcpy(mbr.particion3.status,"i");
    mbr.particion4.comienzo=0;
    mbr.particion4.tamanio=0;
    strcpy(mbr.particion4.nombre,"");
    strcpy(mbr.particion4.status,"i");
    //momento en el que se quiere almacenar el MBR del disco creado
    FILE *archivo;
    archivo = fopen(fullpath, "rb+");
    fseek(archivo, 0, SEEK_SET);
    fwrite(&mbr, sizeof_mbr, 1, archivo);
    fclose(archivo);
    printf("El mbr se va a almacenar con un tamanio: %d\n",mbr.tamanio);
}


//////////////////PARTE PARA FUNCION RMDISK////////////////// 2nda Funcion
int rmdisk(char *path)
{

    remove(path);
    /*char comando[255];
    sprintf(comando,"rm \"%s\"", path);
    printf(comando);
    system(comando);*/
}


//////////////////PARTE PARA FUNCION FDISK////////////////// 2nda Funcion


int fdisk_agregar(int size, char unit, char *path, char type[1], char fit[2], char *name)
{
    //de primero hay que verificar si es que la ruta ingresada es correcta y existe el disco
    struct stat st = {0};
    if (stat(path, &st) == -1)
    {
        printf("Error: el disco al que se intenta acceder no se encuentra\n");
        return 1;
    }
    int tamanio;
    if(unit=='B')
    {
        tamanio=size;
    }
    else if(unit=='K' || unit=='/0')
    {
        tamanio=size*1024;
    }
    else if(unit=='M')
    {
        tamanio=size*1024*1024;
    }
    else
    {
        printf("Error: tipo de unidad de almacenamiento incorrecta\n");
        return 1;
    }

    MBR mbr;
    FILE *archivo;
    archivo = fopen(path, "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof_mbr, 1, archivo);


    if(type=="L")
    {
        int tamanioParticion=0;
        int inicioParticion=0;
        int indiceEbr=0;
        if(strcmp(mbr.particion1.tipo,"E")==0)
        {
            indiceEbr=mbr.particion1.comienzo;
            tamanioParticion=mbr.particion1.tamanio;
            inicioParticion=mbr.particion1.comienzo;
        }

        else if(strcmp(mbr.particion2.tipo,"E")==0)
        {
            indiceEbr=mbr.particion2.comienzo;
            tamanioParticion=mbr.particion2.tamanio;
            inicioParticion=mbr.particion2.comienzo;
        }
        else if(strcmp(mbr.particion3.tipo,"E")==0)
        {
            indiceEbr=mbr.particion3.comienzo;
            tamanioParticion=mbr.particion3.tamanio;
            inicioParticion=mbr.particion3.comienzo;
        }
        else if(strcmp(mbr.particion4.tipo,"E")==0)
        {
            indiceEbr=mbr.particion4.comienzo;
            tamanioParticion=mbr.particion4.tamanio;
            inicioParticion=mbr.particion4.comienzo;
        }
        if(indiceEbr==0)
        {
            printf("Error: No se encontro la particion extendida a la que asignar la particion logica\n");
            return 1;
        }
        EBR ebr;
        int u=0;
        while(u==0)
        {

            fseek(archivo, indiceEbr, SEEK_SET);
            fread(&ebr, sizeof_ebr, 1, archivo);

            //se va a verificar si existe un espacio entre los EBR al que se le pueda insertar el EBR nuevo
            if((indiceEbr+ebr.tamanio+sizeof_ebr)!=ebr.siguiente && ebr.siguiente!=0)
            {
                if((ebr.siguiente-(indiceEbr+ebr.tamanio+sizeof_ebr))>=tamanio)
                {
                    printf("Se encontro un espacio entre las particiones logicas para insertar una nueva\n");
                    //primero se va a crear un nuevo EBR al cual se le asigna sus datos y ademas su siguiente que es el mismo que el temporal, luego a este se le escribe en el documento y al igual que el temporal con el atributo siguiente apuntando al nuevo creado
                    EBR nuevo;
                    strcpy(nuevo.ajuste,fit);
                    strcpy(nuevo.nombre,name);
                    strcpy(nuevo.status, "a");
                    nuevo.tamanio=tamanio;
                    nuevo.siguiente=ebr.siguiente;
                    nuevo.comienzo=(indiceEbr+ebr.tamanio+sizeof_ebr);
                    fseek(archivo, nuevo.comienzo, SEEK_SET);
                    fwrite(&nuevo, sizeof_ebr, 1, archivo);
                    ebr.siguiente=nuevo.comienzo;
                    fseek(archivo, ebr.comienzo, SEEK_SET);
                    fwrite(&ebr, sizeof_ebr, 1, archivo);
                    fclose(archivo);
                    return 1;
                }
            }

            if(strcmp(ebr.status,"i")==0 || ebr.tamanio==0)
            {
                ebr.comienzo=indiceEbr;
                //se verificara si se dispone de espacio suficiente para almacenar la particion logica
                if((indiceEbr+tamanio)>(inicioParticion+tamanioParticion))
                {
                    fclose(archivo);
                    printf("Error: Al intentar almacenar la particion logica se encontro con espacio insuficiente en la particion Extendida del disco\n");
                    return 1;
                }

                printf("Se almacenara la nueva particion logica a partir de la posiscion: %d\n", ebr.comienzo);
                strcpy(ebr.ajuste,fit);
                strcpy(ebr.nombre,name);
                strcpy(ebr.status, "a");
                ebr.tamanio=tamanio;
                fseek(archivo, indiceEbr, SEEK_SET);
                fwrite(&ebr, sizeof_ebr, 1, archivo);
                fclose(archivo);
                return 1;
            }
            if(ebr.siguiente==0)
            {

                EBR nueva;
                nueva.comienzo=ebr.comienzo+sizeof_ebr;
                nueva.siguiente=0;
                nueva.tamanio=0;
                strcpy(mbr.particion1.status,"i");
                fseek(archivo, indiceEbr, SEEK_SET);

                ebr.siguiente=indiceEbr+ebr.tamanio+sizeof_ebr;
                //se va a verificar que la particion extendida tenga suficiente espacio para almacenar el EBR
                if(ebr.siguiente>(indiceEbr+tamanioParticion))
                {
                    printf("Error: el espacio restante de la particion extendida es insuficiente para agregar otra particion logica\n");
                    fclose(archivo);
                    return 1;
                }
                fwrite(&ebr, sizeof_ebr, 1, archivo);
                fseek(archivo, indiceEbr+ebr.tamanio+sizeof_ebr, SEEK_SET);
                fwrite(&nueva, sizeof_ebr, 1, archivo);
            }
            indiceEbr=ebr.siguiente;
        }

        fclose(archivo);

        return 1;
    }


    int y = primerEspacioVacio(mbr);

    printf("Dentro de el disco se encontro que la posicion vacia entre: %d y %d\n",y, proximaPosicion(mbr, y));

    int espacio=proximaPosicion(mbr, y)-y;



    if(espacio>tamanio)
    {
        printf("La unidad del disco posee espacio suficiente para el almacenamiento de la particion\n");
        PARTITION nueva;
        strcpy(nueva.ajuste,fit);
        nueva.comienzo=y;
        strcpy(nueva.nombre,name);
        strcpy(nueva.status, "a");
        nueva.tamanio=tamanio;
        strcpy(nueva.tipo,type);

        if(mbr.particion1.tamanio==0)
        {
            mbr.particion1=nueva;
        }
        else if(mbr.particion2.tamanio==0)
        {
            mbr.particion2=nueva;
        }
        else if(mbr.particion3.tamanio==0)
        {
            mbr.particion3=nueva;
        }
        else if(mbr.particion4.tamanio==0)
        {
            mbr.particion4=nueva;
        }
        else
        {
            printf("Error: ya se ha almacenado las cuatro particiones respectivas en el disco\n");
            return 1;
        }

        FILE *archivo;
        archivo = fopen(path, "rb+");
        fseek(archivo, 0, SEEK_SET);
        fwrite(&mbr, sizeof_mbr, 1, archivo);
        if(type=="E")
        {
            EBR ebr;
            ebr.comienzo=y;
            ebr.siguiente=0;
            ebr.tamanio=0;
            strcpy(ebr.status, "i");
            fseek(archivo, y, SEEK_SET);
            fwrite(&ebr, sizeof_ebr, 1, archivo);
        }
        fclose(archivo);

    }
    else
    {
        printf("La unidad del disco no posee espacio suficiente para el almacenamiento de la particion\n");
        return 1;
    }
}


int fdisk_add(int add, char unit, char *path, char *name)
{
    //de primero hay que verificar si es que la ruta ingresada es correcta y existe el disco
    struct stat st = {0};
    if (stat(path, &st) == -1)
    {
        printf("Error: el disco al que se intenta acceder no se encuentra\n");
        return 1;
    }

    int tamanio;
    if(unit=='B')
    {
        tamanio=add;
    }
    else if(unit=='K' || unit=='/0')
    {
        tamanio=add*1024;
    }
    else if(unit=='M')
    {
        tamanio=add*1024*1024;
    }
    else
    {
        printf("Error: tipo de unidad de almacenamiento incorrecta\n");
        return 1;
    }

    MBR mbr;
    FILE *archivo;
    archivo = fopen(path, "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof_mbr, 1, archivo);


    //numero que obtiene el siguiente espacio ocupado
    int proxima;

    //por si se desea eliminar la particion que ocupa la primera posicion
    if(strcmp(name,mbr.particion1.nombre)==0 && strcmp(mbr.particion1.status, "a")==0)
    {
        //si el tamanio calculado es positivo se va a agregar espacio a la particion
        if(tamanio>0)
        {
            proxima=proximaPosicion(mbr,mbr.particion1.comienzo);
            //si es que existe un espacio vacio despues de la particion
            if(proxima!=(mbr.particion1.comienzo+mbr.particion1.tamanio))
            {
                printf("Si se encontro espacio vacio despues de la particion para que sea agregado\n");
                mbr.particion1.tamanio=mbr.particion1.tamanio+tamanio;
                fseek(archivo, 0, SEEK_SET);
                fwrite(&mbr, sizeof_mbr, 1, archivo);
                fclose(archivo);
                return 1;
            }
            else
            {
                printf("No se encuentra espacio vacio despues de la particion para que sea agregado\n");
                return 1;
            }
        }
        //si se desea eliminar espacio de la particion
        else{
            if((mbr.particion1.tamanio+tamanio)>0){
                printf("Se tratara de reducir el espacio de la particion\n");

            }
            else{
                printf("Error: el espacio es insuficiente en la particion como para realizar la reduccion de espacio solicitada\n");
                fclose(archivo);
                return 1;
            }

        }
    }

    //por si se desea eliminar la particion que ocupa la segunda posicion
    if(strcmp(name,mbr.particion2.nombre)==0 && strcmp(mbr.particion2.status, "a")==0)
    {
        //si el tamanio calculado es positivo se va a agregar espacio a la particion
        if(tamanio>0)
        {
            proxima=proximaPosicion(mbr,mbr.particion1.comienzo);
            //si es que existe un espacio vacio despues de la particion
            if(proxima!=(mbr.particion1.comienzo+mbr.particion1.tamanio))
            {
                printf("Si se encontro espacio vacio despues de la particion para que sea agregado\n");

                return 1;
            }
            else
            {
                printf("No se encuentra espacio vacio despues de la particion para que sea agregado\n");
                return 1;
            }

        }
    }

    //por si se desea eliminar la particion que ocupa la tercera posicion
    if(strcmp(name,mbr.particion3.nombre)==0 && strcmp(mbr.particion3.status, "a")==0)
    {
        //numero que obtiene el siguiente espacio ocupado
        int proxima;
        //si el tamanio calculado es positivo se va a agregar espacio a la particion
        if(tamanio>0)
        {
            proxima=proximaPosicion(mbr,mbr.particion1.comienzo);
            //si es que existe un espacio vacio despues de la particion
            if(proxima!=(mbr.particion1.comienzo+mbr.particion1.tamanio))
            {
                printf("Si se encontro espacio vacio despues de la particion para que sea agregado\n");

                return 1;
            }
            else
            {
                printf("No se encuentra espacio vacio despues de la particion para que sea agregado\n");
                return 1;
            }

        }
    }

    //por si se desea eliminar la particion que ocupa la cuarta posicion
    if(strcmp(name,mbr.particion4.nombre)==0 && strcmp(mbr.particion4.status, "a")==0)
    {
        //**
    }

    //si no encontro ninguna coincidencia anterior va a buscar si la particion que se desea eliminar es de tipo logica
    int inicio;
    if(strcmp(mbr.particion1.tipo, "E")==0)
    {
        inicio=mbr.particion1.comienzo;
    }
    else if(strcmp(mbr.particion2.tipo, "E")==0)
    {
        inicio=mbr.particion2.comienzo;
    }
    else if(strcmp(mbr.particion3.tipo, "E")==0)
    {
        inicio=mbr.particion3.comienzo;
    }
    else if(strcmp(mbr.particion4.tipo, "E")==0)
    {
        inicio=mbr.particion4.comienzo;
    }

    if(inicio==0)
    {
        fclose(archivo);
        printf("Error: No se encontro la particion a eliminar\n");
        return 1;
    }

    EBR ebr;
    fseek(archivo, inicio, SEEK_SET);
    fread(&ebr, sizeof_ebr, 1, archivo);
    //si viene al inicio de la lista de ebr
    if(strcmp(ebr.nombre, name)==0 && strcmp(ebr.status, "a")==0)
    {
        //int proxima=pro;
        //si el tamanio es positivo


        fclose(archivo);
        return 1;
    }

    //de lo contrario se entiende que se encuentra dentro de la lista de ebr
    inicio=ebr.siguiente;

    if(ebr.siguiente!=0)
    {
        while(1==1)
        {

            fseek(archivo, inicio, SEEK_SET);
            fread(&ebr, sizeof_ebr, 1, archivo);
            if(strcmp(ebr.nombre, name)==0 && strcmp(ebr.status, "a")==0)
            {


            }
            if(ebr.siguiente==0)
            {
                printf("No se encontro la particion que se desea eliminar\n");
                fclose(archivo);
                return 1;
            }
            inicio=ebr.siguiente;
        }
    }
    else
    {
        printf("No se encontro la particion que se desea eliminar\n");
        fclose(archivo);
        return 1;
    }
    fclose(archivo);
}



//metodo utilizado para conocer en que punto comienza un espacio vacio
int primerEspacioVacio(struct MBR mbr)
{

    int respuesta=mbr.tamanio;
    //si existe un espacio vacio antes de cualquier particion:
    if(sizeof_mbr!=mbr.particion1.comienzo && sizeof_mbr!=mbr.particion2.comienzo && sizeof_mbr!=mbr.particion3.comienzo && sizeof_mbr!=mbr.particion4.comienzo)
    {
        return sizeof_mbr;
    }
    //si existe un espacio vacio despues de la primera particion
    int auxiliar=(mbr.particion1.comienzo+mbr.particion1.tamanio);
    if(auxiliar!=mbr.particion2.comienzo && auxiliar!=mbr.particion3.comienzo && auxiliar!=mbr.particion4.comienzo && (respuesta>(mbr.particion1.comienzo+mbr.particion1.tamanio)))
    {

        respuesta=mbr.particion1.comienzo+mbr.particion1.tamanio;
    }
    //si existe un espacio vacio despues de la segunda particion
    auxiliar=(mbr.particion2.comienzo+mbr.particion2.tamanio);
    if(auxiliar!=mbr.particion3.comienzo && auxiliar!=mbr.particion4.comienzo && (respuesta>(mbr.particion2.comienzo+mbr.particion2.tamanio)))
    {
        respuesta=mbr.particion2.comienzo+mbr.particion2.tamanio;
    }
    //si existe un espacio vacio despues de la tercera particion
    if((mbr.particion3.comienzo+mbr.particion3.tamanio)!=mbr.particion4.comienzo && (respuesta>(mbr.particion3.comienzo+mbr.particion3.tamanio)))
    {
        respuesta=mbr.particion3.comienzo+mbr.particion3.tamanio;
    }
    //seccion que verifica si es que el espacio vacio viene hasta al final de todas las particiones
    int v=mbr.particion1.comienzo+mbr.particion1.tamanio;
    if((mbr.particion2.comienzo+mbr.particion2.tamanio)>v)
    {
        v=mbr.particion2.comienzo+mbr.particion2.tamanio;
    }
    if((mbr.particion3.comienzo+mbr.particion3.tamanio)>v)
    {
        v=mbr.particion3.comienzo+mbr.particion3.tamanio;
    }
    if((mbr.particion4.comienzo+mbr.particion4.tamanio)>v)
    {
        v=mbr.particion4.comienzo+mbr.particion4.tamanio;
    }
    if(v<respuesta)
    {
        respuesta=v;
    }
    //seccion que verifica si es que es posible rescribir una particion eliminada
    if(mbr.particion1.tamanio!=0 && strcmp(mbr.particion1.status, "i")==0 && respuesta>mbr.particion1.comienzo)
    {
        respuesta=mbr.particion1.comienzo;
    }
    if(mbr.particion2.tamanio!=0 && strcmp(mbr.particion2.status, "i")==0 && respuesta>mbr.particion2.comienzo)
    {
        respuesta=mbr.particion2.comienzo;
    }
    if(mbr.particion3.tamanio!=0 && strcmp(mbr.particion3.status, "i")==0 && respuesta>mbr.particion3.comienzo)
    {
        respuesta=mbr.particion3.comienzo;
    }
    if(mbr.particion4.tamanio!=0 && strcmp(mbr.particion4.status, "i")==0 && respuesta>mbr.particion4.comienzo)
    {
        respuesta=mbr.particion4.comienzo;
    }

    return respuesta;
}

int imprimir()
{

    MBR mbr;
    FILE *archivo;
    archivo = fopen("/home/mis discos/disk2.dsk", "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof_mbr, 1, archivo);

    printf("---IMPRIMIR---\nPARTICION 1\n-Ajuste: %s\n-Comienzo: %d\n-Nombre: %s\n-Status: %s\n-Tamanio: %d\n-Tipo: %s\n-FIN PARTICION 1-\n",mbr.particion1.ajuste,mbr.particion1.comienzo,mbr.particion1.nombre,mbr.particion1.status,mbr.particion1.tamanio,mbr.particion1.tipo);
    printf("PARTICION 2\n-Ajuste: %s\n-Comienzo: %d\n-Nombre: %s\n-Status: %s\n-Tamanio: %d\n-Tipo: %s\n-FIN PARTICION 2-\n",mbr.particion2.ajuste,mbr.particion2.comienzo,mbr.particion2.nombre,mbr.particion2.status,mbr.particion2.tamanio,mbr.particion2.tipo);
    printf("PARTICION 3\n-Ajuste: %s\n-Comienzo: %d\n-Nombre: %s\n-Status: %s\n-Tamanio: %d\n-Tipo: %s\n-FIN PARTICION 3-\n",mbr.particion3.ajuste,mbr.particion3.comienzo,mbr.particion3.nombre,mbr.particion3.status,mbr.particion3.tamanio,mbr.particion3.tipo);
    printf("PARTICION 4\n-Ajuste: %s\n-Comienzo: %d\n-Nombre: %s\n-Status: %s\n-Tamanio: %d\n-Tipo: %s\n-FIN PARTICION 4-\n",mbr.particion4.ajuste,mbr.particion4.comienzo,mbr.particion4.nombre,mbr.particion4.status,mbr.particion4.tamanio,mbr.particion4.tipo);
    //**
    //si no encontro ninguna coincidencia anterior va a buscar si la particion que se desea eliminar es de tipo logica
    int inicio;
    if(strcmp(mbr.particion1.tipo, "E")==0)
    {
        inicio=mbr.particion1.comienzo;
    }
    else if(strcmp(mbr.particion2.tipo, "E")==0)
    {
        inicio=mbr.particion2.comienzo;
    }
    else if(strcmp(mbr.particion3.tipo, "E")==0)
    {
        inicio=mbr.particion3.comienzo;
    }
    else if(strcmp(mbr.particion4.tipo, "E")==0)
    {
        inicio=mbr.particion4.comienzo;
    }

    if(inicio==0)
    {
        fclose(archivo);
        printf("Error: No se encontro la particion a editar\n");
        return 1;
    }

    EBR ebr;
    fseek(archivo, inicio, SEEK_SET);
    fread(&ebr, sizeof_ebr, 1, archivo);

    inicio=ebr.siguiente;

    if(ebr.siguiente!=0)
    {
        while(1==1)
        {

            fseek(archivo, inicio, SEEK_SET);
            fread(&ebr, sizeof_ebr, 1, archivo);

            printf("***Nueva particion logica***\n-Ajuste: %s\n-Comienzo: %d\n-Nombre: %s\n-Siguiente: %d\n-Status: %s\n-Tamanio: %d\n",ebr.ajuste,ebr.comienzo,ebr.nombre,ebr.siguiente,ebr.status,ebr.tamanio);

            if(ebr.siguiente==0)
            {
                fclose(archivo);
                return 1;
            }
            inicio=ebr.siguiente;
        }
    }
    else
    {
        fclose(archivo);
        return 1;
    }

    fclose(archivo);
}

//metodo utilizado para saber hasta donde abarca un espacio vacio
int proximaPosicion(struct MBR mbr, int indice)
{
    int respuesta=mbr.tamanio;
    if(mbr.particion1.tamanio!=0 && mbr.particion1.comienzo>indice && mbr.particion1.comienzo<respuesta)
    {
        respuesta=mbr.particion1.comienzo;
    }
    if(mbr.particion2.tamanio!=0 && mbr.particion2.comienzo>indice && mbr.particion2.comienzo<respuesta)
    {
        respuesta=mbr.particion2.comienzo;
    }
    if(mbr.particion3.tamanio!=0 && mbr.particion3.comienzo>indice && mbr.particion3.comienzo<respuesta)
    {
        respuesta=mbr.particion3.comienzo;
    }
    if(mbr.particion4.tamanio!=0 && mbr.particion4.comienzo>indice && mbr.particion4.comienzo<respuesta)
    {
        respuesta=mbr.particion4.comienzo;
    }
    return respuesta;
}


//CAMBIAR LO DE TAMANIO*******************************************


//metodo utilizado para saber hasta donde abarca un espacio vacio antes del indice enviado
/*int posicionPosterior(struct MBR mbr, int indice)
{
    int respuesta=sizeof_mbr;
    if(mbr.particion1.tamanio!=0 && (mbr.particion1.comienzo + mbr.particion1.tamanio) < indice && (mbr.particion1.comienzo + mbr.particion1.tamanio)>respuesta)
    {
        respuesta=(mbr.particion1.comienzo + mbr.particion1.tamanio);
    }
    if(mbr.particion2.tamanio!=0 && (mbr.particion2.comienzo + mbr.particion2.tamanio) < indice && (mbr.particion2.comienzo + mbr.particion2.tamanio)>respuesta)
    {
        respuesta=(mbr.particion2.comienzo + mbr.particion2.tamanio);
    }
    if(mbr.particion3.tamanio!=0 && (mbr.particion3.comienzo + mbr.particion3.tamanio) < indice && (mbr.particion3.comienzo + mbr.particion3.tamanio)>respuesta)
    {
        respuesta=(mbr.particion3.comienzo + mbr.particion3.tamanio);
    }
    if(mbr.particion4.tamanio!=0 && (mbr.particion4.comienzo + mbr.particion4.tamanio) < indice && (mbr.particion4.comienzo + mbr.particion4.tamanio)>respuesta)
    {
        respuesta=(mbr.particion4.comienzo + mbr.particion4.tamanio);
    }
    return respuesta;
}*/


int fdisk_eliminar(char *path, char *name, char *delete)

{
    MBR mbr;
    FILE *archivo;
    archivo = fopen(path, "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof_mbr, 1, archivo);

    //por si se desea eliminar la particion que ocupa la primera posicion
    if(strcmp(name,mbr.particion1.nombre)==0 && strcmp(mbr.particion3.status, "a")==0)
    {
        if(strcmp(delete, "Fast")==0 || strcmp(delete, "Full")==0)
        {
            strcpy(mbr.particion1.status,"i");
            fseek(archivo,0,SEEK_SET);
            fwrite(&mbr, sizeof_mbr, 1, archivo);

            if(strcmp(delete, "Full")==0)
            {
                char buffer[1];
                int i=0;
                fseek(archivo, mbr.particion1.comienzo,SEEK_SET);

                for(i=0; i<(mbr.particion1.tamanio); i++)
                {
                    fwrite(&buffer,1,1,archivo);
                }
            }
            fclose(archivo);
            printf("Se ha eliminado exitosamente la particion\n");
            return 1;
        }
        else
        {
            printf("Error: el tipo de formato de eliminacion no es el correcto\n");
            fclose(archivo);
            return 1;
        }
    }

    //por si se desea eliminar la particion que ocupa la segunda posicion
    if(strcmp(name,mbr.particion2.nombre)==0 && strcmp(mbr.particion2.status, "a")==0)
    {
        if(strcmp(delete, "Fast")==0 || strcmp(delete, "Full")==0)
        {
            strcpy(mbr.particion2.status,"i");
            fseek(archivo,0,SEEK_SET);
            fwrite(&mbr, sizeof_mbr, 1, archivo);

            if(strcmp(delete, "Full")==0)
            {
                char buffer[1];
                int i=0;
                fseek(archivo, mbr.particion2.comienzo,SEEK_SET);

                for(i=0; i<(mbr.particion2.tamanio); i++)
                {
                    fwrite(&buffer,1,1,archivo);
                }
            }
            fclose(archivo);
            printf("Se ha eliminado exitosamente la particion\n");
            return 1;
        }
        else
        {
            printf("Error: el tipo de formato de eliminacion no es el correcto\n");
            fclose(archivo);
            return 1;
        }
    }

    //por si se desea eliminar la particion que ocupa la tercera posicion
    if(strcmp(name,mbr.particion3.nombre)==0 && strcmp(mbr.particion3.status, "a")==0)
    {
        if(strcmp(delete, "Fast")==0 || strcmp(delete, "Full")==0)
        {
            strcpy(mbr.particion3.status,"i");
            fseek(archivo,0,SEEK_SET);
            fwrite(&mbr, sizeof_mbr, 1, archivo);

            if(strcmp(delete, "Full")==0)
            {
                char buffer[1];
                int i=0;
                fseek(archivo, mbr.particion3.comienzo,SEEK_SET);

                for(i=0; i<(mbr.particion3.tamanio); i++)
                {
                    fwrite(&buffer,1,1,archivo);
                }
            }
            fclose(archivo);
            printf("Se ha eliminado exitosamente la particion\n");
            return 1;
        }
        else
        {
            printf("Error: el tipo de formato de eliminacion no es el correcto\n");
            fclose(archivo);
            return 1;
        }
    }

    //por si se desea eliminar la particion que ocupa la cuarta posicion
    if(strcmp(name,mbr.particion4.nombre)==0 && strcmp(mbr.particion4.status, "a")==0)
    {
        if(strcmp(delete, "Fast")==0 || strcmp(delete, "Full")==0)
        {
            strcpy(mbr.particion4.status,"i");
            fseek(archivo,0,SEEK_SET);
            fwrite(&mbr, sizeof_mbr, 1, archivo);

            if(strcmp(delete, "Full")==0)
            {
                char buffer[1];
                int i=0;
                fseek(archivo, mbr.particion4.comienzo,SEEK_SET);

                for(i=0; i<(mbr.particion4.tamanio); i++)
                {
                    fwrite(&buffer,1,1,archivo);
                }
            }
            fclose(archivo);
            printf("Se ha eliminado exitosamente la particion\n");
            return 1;
        }
        else
        {
            printf("Error: el tipo de formato de eliminacion no es el correcto\n");
            fclose(archivo);
            return 1;
        }
    }

    //si no encontro ninguna coincidencia anterior va a buscar si la particion que se desea eliminar es de tipo logica
    int inicio;
    if(strcmp(mbr.particion1.tipo, "E")==0)
    {
        inicio=mbr.particion1.comienzo;
    }
    else if(strcmp(mbr.particion2.tipo, "E")==0)
    {
        inicio=mbr.particion2.comienzo;
    }
    else if(strcmp(mbr.particion3.tipo, "E")==0)
    {
        inicio=mbr.particion3.comienzo;
    }
    else if(strcmp(mbr.particion4.tipo, "E")==0)
    {
        inicio=mbr.particion4.comienzo;
    }

    if(inicio==0)
    {
        fclose(archivo);
        printf("Error: No se encontro la particion a eliminar\n");
        return 1;
    }

    EBR ebr;
    fseek(archivo, inicio, SEEK_SET);
    fread(&ebr, sizeof_ebr, 1, archivo);
    //si viene al inicio de la lista de ebr
    if(strcmp(ebr.nombre, name)==0 && strcmp(ebr.status, "a")==0)
    {
        printf("Se eliminara la primera particion logica del disco\n");
        if(strcmp(delete, "Fast")==0 || strcmp(delete, "Full")==0)
        {
            strcpy(ebr.status,"i");
            fseek(archivo, inicio, SEEK_SET);
            fwrite(&ebr, sizeof_ebr, 1, archivo);

            if(strcmp(delete, "Full")==0)
            {
                char buffer[1];
                int i=0;
                fseek(archivo, ebr.comienzo+sizeof_ebr,SEEK_SET);

                for(i=0; i<(ebr.tamanio); i++)
                {
                    fwrite(&buffer,1,1,archivo);
                }
            }
            fclose(archivo);
            printf("Se ha eliminado exitosamente la particion logica\n");
            return 1;
        }
        else
        {
            printf("Error: el tipo de formato de eliminacion no es el correcto\n");
            fclose(archivo);
            return 1;
        }
        fclose(archivo);
        return 1;
    }

    //de lo contrario se entiende que se encuentra dentro de la lista de ebr
    inicio=ebr.siguiente;

    if(ebr.siguiente!=0)
    {
        while(1==1)
        {

            fseek(archivo, inicio, SEEK_SET);
            fread(&ebr, sizeof_ebr, 1, archivo);
            if(strcmp(ebr.nombre, name)==0 && strcmp(ebr.status, "a")==0)
            {
                if(strcmp(delete, "Fast")==0 || strcmp(delete, "Full")==0)
                {
                    printf("Se encontro la particion logica a eliminar\n");
                    strcpy(ebr.status,"i");
                    fseek(archivo,inicio,SEEK_SET);
                    fwrite(&ebr, sizeof_ebr, 1, archivo);

                    if(strcmp(delete, "Full")==0)
                    {
                        char buffer[1];
                        int i=0;
                        fseek(archivo, ebr.comienzo+sizeof_ebr,SEEK_SET);

                        for(i=0; i<(ebr.tamanio); i++)
                        {
                            fwrite(&buffer,1,1,archivo);
                        }
                    }
                    fclose(archivo);
                    printf("Se ha eliminado exitosamente la particion logica\n");
                    return 1;
                }
                else
                {
                    printf("Error: el tipo de formato de eliminacion no es el correcto\n");
                    fclose(archivo);
                    return 1;
                }

            }
            if(ebr.siguiente==0)
            {
                printf("No se encontro la particion que se desea eliminar\n");
                fclose(archivo);
                return 1;
            }
            inicio=ebr.siguiente;
        }
    }
    else
    {
        printf("No se encontro la particion que se desea eliminar\n");
        fclose(archivo);
        return 1;
    }
    fclose(archivo);
}


reporteMbr()
{
    /*MBR mbr;
    FILE *archivo;
    archivo = fopen(path, "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof_mbr, 1, archivo);*/


}





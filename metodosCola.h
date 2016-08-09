#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct nodoParticion
{
//LOS APUNTADORES A NODOS
    struct  nodoParticion *siguiente;
//LOS ATRIBUTOS
    char *path;
    char *nombre;
    char *id;
} PARTICION;

PARTICION *inicio=NULL;
/////////////////////ESPACIO PARA CONTROLAR LOS METOOS DE LA COLA DE ESPERA DE CARRETA QUE CONTIENE A LOS CLIENTES/////////////////////

void mount (char *path, char *nombre, char *id)
{
    if(fdisk_buscar(path,nombre)==1)
    {
        printf("Se encontro la particion que se desea montar\n");
        PARTICION *auxiliar =(PARTICION *)malloc(sizeof(PARTICION));
        auxiliar->id=id;
        auxiliar->nombre=nombre;
        auxiliar->path=path;
        auxiliar->siguiente=NULL;
        agregarACola(auxiliar);
    }
    else
    {
        printf("No se encontro la particion que se desea montar\n");
    }
}

void agregarACola(PARTICION *nuevo)
{

    if (estaVacio()==1)
    {
        inicio=nuevo;
    }
    else
    {
        PARTICION *auxiliar =(PARTICION *)malloc(sizeof(PARTICION));
        auxiliar=inicio;
        while(auxiliar->siguiente!=NULL)
        {
            auxiliar=auxiliar->siguiente;
        }
        auxiliar->siguiente=nuevo;
    }
}

int estaVacio()
{
    if(inicio==NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void leerLista()
{
    PARTICION *auxiliar =(PARTICION *)malloc(sizeof(PARTICION));
    auxiliar=inicio;

    while(auxiliar!=NULL)
    {
        printf("Dentro de la estructura hay una particion: %s\n",auxiliar->id);

        if(auxiliar->siguiente==NULL)
        {
            return;
        }
        auxiliar=auxiliar->siguiente;
    }

}


int  reportesdisk()
{
    //de primero hay que verificar si es que la ruta ingresada es correcta y existe el disco
    /*struct stat st = {0};
    if (stat(path, &st) == -1)
    {
        printf("Error: el disco al que se intenta acceder no se encuentra\n");
        return 1;
    }*/

    MBR mbr;
    FILE *archivo;
    archivo = fopen("/home/mis discos/disk2.dsk", "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof_mbr, 1, archivo);



    //numero que obtiene el siguiente espacio ocupado
    FILE *reporte = fopen("/home/rudy/Escritorio/reporte_disk.dot", "w+");
    fprintf(reporte, "digraph particiones{\nrankdir=TB;\n node[shape = record];\nsubgraph cluster{\n");
    fprintf(reporte,"label=\"Reporte\"");
    fprintf(reporte,"nodeGrafica[label=\"MBR");

    PARTITION anterior;

    int actual=sizeof_mbr;
    int i=0;

    PARTITION auxiliar;

    for(i=0; i<4; i++)
    {


        int indice = mbr.tamanio;
        int verificacion=0;
        if(mbr.particion1.comienzo<indice && mbr.particion1.comienzo>=actual )
        {
            auxiliar=mbr.particion1;
            indice=mbr.particion1.comienzo;
            if(actual!=mbr.particion1.comienzo)
            {
                verificacion=1;
            }
        }
        if(mbr.particion2.comienzo<indice && mbr.particion2.comienzo>=actual)
        {
            verificacion=0;
            auxiliar=mbr.particion2;
            indice=mbr.particion2.comienzo;
            if(actual!=mbr.particion2.comienzo)
            {
                verificacion=1;
            }
        }
        if(mbr.particion3.comienzo<indice && mbr.particion3.comienzo>=actual)
        {
            verificacion=0;
            auxiliar=mbr.particion3;
            indice=mbr.particion3.comienzo;
            if(actual!=mbr.particion3.comienzo)
            {
                verificacion=1;
            }
        }
        if(mbr.particion4.comienzo<indice && mbr.particion4.comienzo>=actual)
        {
            verificacion=0;
            auxiliar=mbr.particion4;
            indice=mbr.particion4.comienzo;
            if(actual!=mbr.particion4.comienzo)
            {
                verificacion=1;
            }
        }
        if(indice==mbr.tamanio)
        {
            i=4;
        }
        printf("El valor de verificacion es: %d\n",verificacion);
        if(verificacion==1)
        {
            fprintf(reporte,"|Vacio");
        }
        printf("EL TIPO ES: %s\n",auxiliar.tipo);
        if(strcmp(auxiliar.tipo,"P")==0)
        {
            fprintf(reporte,"|Primaria");
        }
        else if(strcmp(auxiliar.tipo,"E")==0)
        {
            fprintf(reporte,"|{extendida|{");

            //METODO PARA RECUPERAR LAS LOGICAS

            EBR ebr;
            fseek(archivo, auxiliar.comienzo, SEEK_SET);
            fread(&ebr, sizeof_ebr, 1, archivo);
            //si viene al inicio de la lista de ebr
            if(strcmp(ebr.status, "a")==0)
            {
                fprintf(reporte,"EBR|Logica|");
            }
            else{
                fprintf(reporte,"EBR|Logica|");
            }

            //de lo contrario se entiende que se encuentra dentro de la lista de ebr
            inicio=ebr.siguiente;

            if(ebr.siguiente!=0)
            {
                while(1==1)
                {
                    fseek(archivo, inicio, SEEK_SET);
                    fread(&ebr, sizeof_ebr, 1, archivo);
                    fprintf(reporte,"EBR|Logica|");

                    if(ebr.siguiente==0)
                    {
                        break;
                    }
                    inicio=ebr.siguiente;
                }
            }


            //

            fprintf(reporte,"}}");
        }

        actual=auxiliar.comienzo+auxiliar.tamanio;
    }

    if(i>0 && auxiliar.comienzo+auxiliar.tamanio!=mbr.tamanio)
    {
        fprintf(reporte,"|Vacio");
    }


    fprintf(reporte,"\"];\n\t}\n}");
    fclose(reporte);

    char consola[300];
    strcpy(consola,"");
    strcat(consola, "dot -Tpng /home/rudy/Escritorio/reporte_disk.dot  -o /home/rudy/Escritorio/reporte_disk.png");
    system(consola);
    printf("se creo el reporte");
    char consola2[300];
    strcpy(consola2,"");
    strcat(consola2, "xdg-open /home/rudy/Escritorio/reporte_disk.png");
    system(consola2);

    fclose(archivo);
}

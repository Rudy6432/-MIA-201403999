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

void unmount(char *id)
{
    PARTICION *auxiliar =(PARTICION *)malloc(sizeof(PARTICION));
    auxiliar=inicio;

    if(inicio==NULL)
    {
        printf("Error: No se encontro la particion a desmontar\n");
        return;
    }
    //si se quiere desmontar el inicio
    if(strcmp(inicio->id, id)==0)
    {
        inicio=inicio->siguiente;
        return;
    }

    while(auxiliar!=NULL)
    {
        if(strcmp(auxiliar->siguiente->id, id)==0)
        {
            //si viene al final
            if(auxiliar->siguiente->siguiente==NULL)
            {
                auxiliar->siguiente=NULL;
                return;
            }
            //si viene en medio
            auxiliar->siguiente=auxiliar->siguiente->siguiente;
        }

        if(auxiliar->siguiente==NULL)
        {
            printf("Error: no se encontro la particion a desmontar\n");
            return;
        }
        auxiliar=auxiliar->siguiente;
    }

}


int  reportesdisk(char *path, char *id)
{
    struct stat st = {0};

    if (stat(path, &st) == -1)
    {
        printf("La carpeta destino no existe, se proseguira a crear una nueva\n");
        char *command="mkdir \"";
        char *comando = (char *) malloc(1 + strlen(command)+ strlen(path));
        strcpy(comando, command);
        strcat(comando, path);
        strcat(comando, "\"");
        printf("%s\n", comando);
    }

    char ruta[100];
    strcpy(ruta,"");

    //Seccion para recuperar la direccion del disco al que se quiere
    PARTICION *aux =(PARTICION *)malloc(sizeof(PARTICION));
    aux=inicio;

    if(inicio==NULL)
    {
        printf("Error: No se encontro el disco a analizar\n");
        return;
    }

    while(aux!=NULL)
    {
        if(strcmp(aux->id,id)==0)
        {
            strcpy(ruta,aux->path);
            break;
        }

        if(aux->siguiente==NULL)
        {
            printf("Error: no se encontro la particion con lo que se quiere realizar el reporte\n");
            break;
        }
        aux=aux->siguiente;
    }
    //

    MBR mbr;
    FILE *archivo;
    archivo = fopen(ruta, "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof_mbr, 1, archivo);



    //numero que obtiene el siguiente espacio ocu/reporte_disk.dotpado
    char rutaImagen[100];
    sprintf(rutaImagen, "%s/reporte_disk.dot",path);
    printf("CC %s\n",rutaImagen);
    FILE *reporte = fopen(rutaImagen, "w+");
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
            break;
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
            else
            {
                fprintf(reporte,"EBR|Vacia|");
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
    sprintf(consola, "dot -Tpng \"%s/reporte_disk.dot\"  -o \"%s/reporte_disk.png\"",path,path);
    printf("COMANDO: %s\n",consola);
    system(consola);
    printf("se creo el reporte");
    sprintf(consola, "xdg-open \"%s/reporte_disk.png\"",path);
    system(consola);

    fclose(archivo);
}

int  reportesmbr(char *path, char *id)
{
    char datos[999999];

    struct stat st = {0};

    if (stat(path, &st) == -1)
    {
        printf("La carpeta destino no existe, se proseguira a crear una nueva\n");
        char *command="mkdir \"";
        char *comando = (char *) malloc(1 + strlen(command)+ strlen(path));
        strcpy(comando, command);
        strcat(comando, path);
        strcat(comando, "\"");
        printf("%s\n", comando);
    }

    char ruta[100];
    strcpy(ruta,"");

    //Seccion para recuperar la direccion del disco al que se quiere
    PARTICION *aux =(PARTICION *)malloc(sizeof(PARTICION));
    aux=inicio;

    if(inicio==NULL)
    {
        printf("Error: No se encontro el disco a analizar\n");
        return;
    }

    while(aux!=NULL)
    {
        if(strcmp(aux->id,id)==0)
        {
            strcpy(ruta,aux->path);
            break;
        }

        if(aux->siguiente==NULL)
        {
            printf("Error: no se encontro la particion con lo que se quiere realizar el reporte\n");
            break;
        }
        aux=aux->siguiente;
    }
    //

    MBR mbr;
    FILE *archivo;
    archivo = fopen(ruta, "rb+");
    fseek(archivo, 0, SEEK_SET);
    fread(&mbr, sizeof_mbr, 1, archivo);



    //numero que obtiene el siguiente espacio ocu/reporte_disk.dotpado
    char rutaImagen[100];
    sprintf(rutaImagen, "%s/reporte_mbr.dot",path);
    printf("CS %s\n",rutaImagen);

    //char *ebrs;

    FILE *reporte = fopen(rutaImagen, "w+");
    fprintf(reporte, "digraph i{node [shape=plaintext];\n subgraph clusterMBR{label=\"MBR\";\nnode [shape=plaintext]; \na [label=<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">");
    fprintf(reporte,"<tr><td width=\"70%\"><b>Nombre</b></td><td width=\"70%\"><b>Valor</b></td></tr>");
    fprintf(reporte,"<tr><td width=\"70%\"><b>mbr_tamanio</b></td><td width=\"70%\"><b>%d</b></td></tr>",mbr.tamanio);
    fprintf(reporte,"<tr><td width=\"70%\"><b>mbr_fecha_creacion</b></td><td width=\"70%\"><b>%s</b></td></tr>",mbr.fecha_creacion);
    fprintf(reporte,"<tr><td width=\"70%\"><b>mbr_disk_signature</b></td><td width=\"70%\"><b>%d</b></td></tr>",mbr.disk_signature);

    PARTITION anterior;

    int actual=sizeof_mbr;
    int i=0;

    PARTITION auxiliar;

    for(i=0; i<4; i++)
    {


        int indice = mbr.tamanio;
        int x=0;
        if(mbr.particion1.comienzo<indice && mbr.particion1.comienzo>=actual )
        {
            x=1;
            auxiliar=mbr.particion1;
            indice=mbr.particion1.comienzo;
            /*if(actual!=mbr.particion1.comienzo)
            {
                verificacion=1;
            }*/
        }
        if(mbr.particion2.comienzo<indice && mbr.particion2.comienzo>=actual)
        {
            x=2;
            auxiliar=mbr.particion2;
            indice=mbr.particion2.comienzo;
            /*if(actual!=mbr.particion2.comienzo)
            {
                verificacion=1;
            }*/
        }
        if(mbr.particion3.comienzo<indice && mbr.particion3.comienzo>=actual)
        {
            x=3;
            auxiliar=mbr.particion3;
            indice=mbr.particion3.comienzo;
            /*if(actual!=mbr.particion3.comienzo)
            {
                verificacion=1;
            }*/
        }
        if(mbr.particion4.comienzo<indice && mbr.particion4.comienzo>=actual)
        {
            x=4;
            auxiliar=mbr.particion4;
            indice=mbr.particion4.comienzo;
            /*if(actual!=mbr.particion4.comienzo)
            {
                verificacion=1;
            }*/
        }
        if(indice==mbr.tamanio)
        {
            break;
        }
        //printf("El valor de verificacion es: %d\n",verificacion);
        /*if(verificacion==1)
        {
            fprintf(reporte,"|Vacio");
        }*/
        //printf("EL TIPO ES: %s\n",auxiliar.tipo);
        if(strcmp(auxiliar.tipo,"P")==0)
        {
            fprintf(reporte,"<tr><td width=\"70%\"><b>part_status_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",x,auxiliar.status);
            fprintf(reporte,"<tr><td width=\"70%\"><b>part_type_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",x,auxiliar.tipo);
            fprintf(reporte,"<tr><td width=\"70%\"><b>part_fit_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",x,auxiliar.ajuste);
            fprintf(reporte,"<tr><td width=\"70%\"><b>part_start_%d</b></td><td width=\"70%\"><b>%d\</b></td></tr>",x,auxiliar.comienzo);
            fprintf(reporte,"<tr><td width=\"70%\"><b>part_size_%d</b></td><td width=\"70%\"><b>%d\</b></td></tr>",x,auxiliar.tamanio);
            fprintf(reporte,"<tr><td width=\"70%\"><b>part_name_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",x,auxiliar.nombre);
        }
        else if(strcmp(auxiliar.tipo,"E")==0)
        {
//            sprintf(datos," ");

            fprintf(reporte,"<tr><td width=\"70%\"><b>part_status_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",x,auxiliar.status);
            fprintf(reporte,"<tr><td width=\"70%\"><b>part_type_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",x,auxiliar.tipo);
            fprintf(reporte,"<tr><td width=\"70%\"><b>part_fit_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",x,auxiliar.ajuste);
            fprintf(reporte,"<tr><td width=\"70%\"><b>part_start_%d</b></td><td width=\"70%\"><b>%d\</b></td></tr>",x,auxiliar.comienzo);
            fprintf(reporte,"<tr><td width=\"70%\"><b>part_size_%d</b></td><td width=\"70%\"><b>%d\</b></td></tr>",x,auxiliar.tamanio);
            fprintf(reporte,"<tr><td width=\"70%\"><b>part_name_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",x,auxiliar.nombre);

            //**

            //METODO PARA RECUPERAR LAS LOGICAS

            EBR ebr;
            fseek(archivo, auxiliar.comienzo, SEEK_SET);
            fread(&ebr, sizeof_ebr, 1, archivo);
            int y=1;
            //si viene al inicio de la lista de ebr
            if(ebr.tamanio!=0)
            {
                sprintf(datos,"subgraph cluster%d{label=\"EBR_%d\";\nnode%d[label=<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n",y,y,y);
                sprintf(datos, "%s<tr><td width=\"70%\"><b>Nombre</b></td><td width=\"70%\"><b>Valor\</b></td></tr>",datos);
                sprintf(datos, "%s<tr><td width=\"70%\"><b>part_status_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",datos,y,ebr.status);
                sprintf(datos, "%s<tr><td width=\"70%\"><b>part_fit_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",datos,y,ebr.ajuste);
                sprintf(datos, "%s<tr><td width=\"70%\"><b>part_start_%d</b></td><td width=\"70%\"><b>%d\</b></td></tr>",datos,y,ebr.comienzo);
                sprintf(datos, "%s<tr><td width=\"70%\"><b>part_size_%d</b></td><td width=\"70%\"><b>%d\</b></td></tr>",datos,y,ebr.tamanio);
                sprintf(datos, "%s<tr><td width=\"70%\"><b>part_next_%d</b></td><td width=\"70%\"><b>%d\</b></td></tr>",datos,y,ebr.siguiente);
                sprintf(datos, "%s<tr><td width=\"70%\"><b>part_name_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr></table>>];}\n",datos,y,ebr.nombre);
                y=y+1;
            }

            //de lo contrario se entiende que se encuentra dentro de la lista de ebr
            inicio=ebr.siguiente;

            if(ebr.siguiente!=0)
            {
                while(1==1)
                {
                    EBR ebr;
                    fseek(archivo, inicio, SEEK_SET);
                    fread(&ebr, sizeof_ebr, 1, archivo);
                    sprintf(datos,"%ssubgraph cluster%d{label=\"EBR_%d\";\nnode%d[label=<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n",datos,y,y,y);
                    sprintf(datos, "%s<tr><td width=\"70%\"><b>Nombre</b></td><td width=\"70%\"><b>Valor\</b></td></tr>",datos);
                    sprintf(datos, "%s<tr><td width=\"70%\"><b>part_status_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",datos,y,ebr.status);
                    sprintf(datos, "%s<tr><td width=\"70%\"><b>part_fit_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr>",datos,y,ebr.ajuste);
                    sprintf(datos, "%s<tr><td width=\"70%\"><b>part_start_%d</b></td><td width=\"70%\"><b>%d\</b></td></tr>",datos,y,ebr.comienzo);
                    sprintf(datos, "%s<tr><td width=\"70%\"><b>part_size_%d</b></td><td width=\"70%\"><b>%d\</b></td></tr>",datos,y,ebr.tamanio);
                    sprintf(datos, "%s<tr><td width=\"70%\"><b>part_next_%d</b></td><td width=\"70%\"><b>%d\</b></td></tr>",datos,y,ebr.siguiente);
                    sprintf(datos, "%s<tr><td width=\"70%\"><b>part_name_%d</b></td><td width=\"70%\"><b>%s\</b></td></tr></table>>];}\n",datos,y,ebr.nombre);
                    y=y+1;

                    if(ebr.siguiente==0)
                    {
                        break;
                    }
                    inicio=ebr.siguiente;
                }
            }

            //**
        }

        actual=auxiliar.comienzo+auxiliar.tamanio;
    }

    fprintf(reporte,"</table>>];}\n");
    //
    printf(">>%s\n",datos);

    fprintf(reporte,datos);
    fprintf(reporte,"}");



    fclose(reporte);

    char consola[300];
    sprintf(consola, "dot -Tpng \"%s/reporte_mbr.dot\"  -o \"%s/reporte_mbr.png\"",path,path);
    printf("COMANDO: %s\n",consola);
    system(consola);
    printf("se creo el reporte");
    sprintf(consola, "xdg-open \"%s/reporte_disk.png\"",path);
    system(consola);

    fclose(archivo);
}

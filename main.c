#include <stdio.h>
#include <stdlib.h>
#include "AdminDisco.h"
#include "metodosCola.h"
int main()
{
    //system("sudo chmod -R ugo+rw /home/");
    //rmdisk("/home/mis discos/disk2.dsk");
    char unit[10];
    char path[400];
    char name[100];

    sprintf(unit,"M");
    sprintf(path,"/home/mis discos/");
    sprintf(name,"disk2.dsk");

    mkdisk(28,unit,path,name);
    /*fdisk_agregar(12,"M","/home/mis discos/disk2.dsk","E","WF","Extendida1");
    fdisk_agregar(6,"M","/home/mis discos/disk2.dsk","P","WF","Primaria1");
    fdisk_agregar(3,"M","/home/mis discos/disk2.dsk","P","WF","Primaria2");
    imprimir();
    fdisk_add(-3, "M", "/home/mis discos/disk2.dsk", "Primaria1");

    fdisk_agregar(1,"M","/home/mis discos/disk2.dsk","P","WF","Primaria3");
    //imprimir();
    fdisk_agregar(7,"M","/home/mis discos/disk2.dsk","L","WF","Logica1");
    fdisk_agregar(3,"M","/home/mis discos/disk2.dsk","L","WF","Logica4");
    //fdisk_agregar(6,"K","/home/mis discos/disk2.dsk","L","WF","Logica5");
    imprimir();
    fdisk_add(-3, "K", "/home/mis discos/disk2.dsk", "Logica3");
    //imprimir();
    fdisk_agregar(6,"K","/home/mis discos/disk2.dsk","L","jo","Logica007");

    mount("/home/mis discos/disk2.dsk","Extendida1","vda1");
    mount("/home/mis discos/disk2.dsk","Logica5","vda2");
    //leerLista();
    fdisk_eliminar("/home/mis discos/disk2.dsk","Primaria1","Fast");
    fdisk_add(-3, "M", "/home/mis discos/disk2.dsk", "Extendida1");
    imprimir();
    //reportesdisk("/home/mis discos","vda1");
    reportesmbr("/home/mis discos","vda1");

    //fdisk_agregar(3,'K',"/home/mis discos/disk2.dsk","L","jo","Logica900");*/
    return 0;
}

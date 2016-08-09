#include <stdio.h>
#include <stdlib.h>
#include "AdminDisco.h"
#include "metodosCola.h"
int main()
{
    //system("sudo chmod -R ugo+rw /home/");
    rmdisk("/home/mis discos/disk2.dsk");
    mkdisk(18,'m',"/home/mis discos/","disk2.dsk");
    fdisk_agregar(10,'M',"/home/mis discos/disk2.dsk","E","jo","Extendida1");
    fdisk_agregar(6,'M',"/home/mis discos/disk2.dsk","P","jo","Primaria1");
    fdisk_agregar(1,'M',"/home/mis discos/disk2.dsk","P","jo","Primaria2");
    imprimir();
    fdisk_add(-3, 'M', "/home/mis discos/disk2.dsk", "Primaria1");

    fdisk_agregar(1,'M',"/home/mis discos/disk2.dsk","P","jo","Primaria3");
    imprimir();
    fdisk_agregar(7,'M',"/home/mis discos/disk2.dsk","L","jo","Logica3");
    fdisk_agregar(3,'K',"/home/mis discos/disk2.dsk","L","jo","Logica4");
    fdisk_agregar(6,'K',"/home/mis discos/disk2.dsk","L","jo","Logica5");
    imprimir();
    fdisk_add(-3, 'M', "/home/mis discos/disk2.dsk", "Logica3");
    imprimir();
    fdisk_agregar(6,'K',"/home/mis discos/disk2.dsk","L","jo","Logica007");

    mount("/home/mis discos/disk2.dsk","Extendida1","vda1");
    mount("/home/mis discos/disk2.dsk","Logica5","vda2");
    leerLista();
    fdisk_eliminar("/home/mis discos/disk2.dsk","Primaria1","Fast");
    fdisk_add(-3, 'M', "/home/mis discos/disk2.dsk", "Extendida1");
    imprimir();
    reportesdisk();

    //fdisk_agregar(3,'K',"/home/mis discos/disk2.dsk","L","jo","Logica900");
    return 0;
}

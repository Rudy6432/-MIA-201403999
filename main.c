#include <stdio.h>
#include <stdlib.h>
#include "AdminDisco.h"
int main()
{
    rmdisk("/home/mis discos/disk2.dsk");
    mkdisk(18,'m',"/home/mis discos/","disk2.dsk");
    fdisk_agregar(6,'M',"/home/mis discos/disk2.dsk","P","jo","Primaria1");
    fdisk_agregar(1,'M',"/home/mis discos/disk2.dsk","P","jo","Primaria2");
    fdisk_agregar(6,'M',"/home/mis discos/disk2.dsk","E","jo","Extendida1");
    fdisk_agregar(8,'K',"/home/mis discos/disk2.dsk","L","jo","Logica1");
    fdisk_agregar(6,'K',"/home/mis discos/disk2.dsk","L","jo","Logica2");

    //fdisk_agregar(3,'K',"/home/mis discos/disk2.dsk","L","jo","Logica3");
    //fdisk_agregar(3,'K',"/home/mis discos/disk2.dsk","L","jo","Logica4");
    //fdisk_agregar(6,'K',"/home/mis discos/disk2.dsk","L","jo","Logica5");
    imprimir();
    fdisk_add(1, 'K', "/home/mis discos/disk2.dsk", "Primaria1");
    //fdisk_eliminar("/home/mis discos/disk2.dsk", "Logica2", "Fast");

    //fdisk_agregar(3,'K',"/home/mis discos/disk2.dsk","L","jo","Logica900");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "AdminDisco.h"
int main()
{
    /*rmdisk("/home/mis discos/disk2.dsk");
    mkdisk(10,'m',"/home/mis discos/","disk2.dsk");
    fdisk_agregar(6,'M',"/home/mis discos/disk2.dsk","E","jo","Extendida1");
    fdisk_agregar(8,'K',"/home/mis discos/disk2.dsk","L","jo","Logica1");
    fdisk_agregar(6,'K',"/home/mis discos/disk2.dsk","L","jo","Logica2");
    fdisk_agregar(3,'M',"/home/mis discos/disk2.dsk","L","jo","Logica3");*/
    fdisk_agregar(3,'M',"/home/mis discos/disk2.dsk","L","jo","Logica4");
    fdisk_eliminar("/home/mis discos/disk2.dsk", "Logica1", "Fast");





    return 0;
}

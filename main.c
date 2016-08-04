#include <stdio.h>
#include <stdlib.h>
#include "AdminDisco.h"
int main()
{
    rmdisk("/home/mis discos/disk2.dsk");
    mkdisk(10,'m',"/home/mis discos/","disk2.dsk");
    fdisk_agregar(6,'M',"/home/mis discos/disk2.dsk","E","jo","Rudy");
    fdisk_agregar(8,'K',"/home/mis discos/disk2.dsk","L","jo","Rudy");
    fdisk_agregar(6,'K',"/home/mis discos/disk2.dsk","L","jo","Rudy");
    fdisk_agregar(3,'M',"/home/mis discos/disk2.dsk","L","jo","Rudy");





    return 0;
}

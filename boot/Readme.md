# The `boot` folder

Make the boot sector, load kernel to memeory and jump to protected mode.

## File Structures

* `include` include all the `.inc` files that `boot.asm` and `loader.asm` will use
    * `fat12hdr.inc`  the header of FAT12 disk
    * `lib32.inc`  some useful functions for real mode and proctected mode
    * `load.inc`  some constant that `boot.asm` and `loader.asm` both will use
    * `pm.inc` some constant and macros about descriptor, gate and their attributes
* `boot.asm` the boot sector of FAT12 disk, it will load `loader.bin` to memory and jump there
* `loader.asm` find kernel file, jump to protected mode, check hardware info, setup paging and then load and jump to the kernel
* `Readme.md` this file
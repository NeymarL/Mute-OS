# The `boot` directory 

Make the boot sector, load kernel to memeory and jump to protected mode.

## File Structures

* `include` conlude all the `.inc` files that `boot.asm` and `loader.asm` will use
    * `fat12hdr.inc`  the header of FAT12 disk
    * `lib32.inc`  some useful functions for real mode and proctected mode
    * `load.inc`  some constant defined there that `boot.asm` and `loader.asm` both used
    * `pm.inc` some constant and macros about descriptor, gate and attributes
* `boot.asm` the boot sector of FAT12 disk, it will load `loader.bin` to memory and jump there
* `loader.asm` find kernel file, jump to protected mode, check hardware info, setup paging, load and jump to kernel
* `Readme.md` this file
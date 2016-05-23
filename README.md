# ChronOS
Hobbyist Operating System
# Prerequisites
A cross compiler compiled to be able to use a custom sysroot.
The Grub 2 bootloader installed to have the ```grub-mkrescue``` for generating the ISO of the operating system.
To test the OS you need to use a virtualizer/emulator, I recommend QEMU.
# Compilation
To build the OS, you run make which configures, builds the projects, and creates an ISO containing the bootloader, kernel and an initrd. And you run it via QEMU.
```sh
make
qemu-system-i386 -cdrom chronos.iso
```
Or you simply run the qemu make target which builds the project and runs it directly.
```sh
make qemu
```

To build and run directly with QEMU, you can also run the following shell script.
```sh
./qemu.sh
```

To clean the project you simply run the clean make target like so.
```sh
make clean
```
Or manually run the following script.
```sh
./clean.sh
```

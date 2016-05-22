# ChronOS
Hobbyist Operating System
# Prerequisites
A cross compiler compiled to be able to use a custom sysroot
The Grub 2 bootloader installed to have the ```grub-mkrescue``` for generating the ISO of the operating system
# Compilation
To build the OS, you run the following script which configures, builds the projects, and creates an ISO containing the bootloader, kernel and an initrd
```sh
./iso.sh
```

To clean the project you simply run the following script
```sh
./clean.sh
```

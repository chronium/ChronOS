# ChronOS
Hobbyist Operating System
# Prerequisites
A cross compiler compiled to be able to use a custom sysroot.
The Grub 2 bootloader installed to have ```grub-mkrescue``` for generating the ISO of the operating system.
To test the OS you need to use a virtualizer/emulator, I recommend QEMU.
# Compilation
To build the OS, you run make which configures, builds the projects, and creates an ISO containing the bootloader, kernel and an initrd. And you run it via QEMU.
```sh
make
qemu-system-i386 -cdrom chronos.iso
```
Or you simply run the run make target which builds the project and runs it directly.
```sh
make run
```

To clean the project you simply run the clean make target like so.
```sh
make clean
```

# Troubleshooting
## `xorriso : FAILURE : Cannot find path '/efi.img' in loaded ISO image`

**Linux**:
* On Arch: `sudo pacman -Sy mtools`
* On Debian/Ubuntu: `sudo apt-get install mtools`
* On Fedora/Red Hat/CentOS: `sudo yum install mtools`

## `grub-mkrescue: error: ``mformat`` invocation failed`
See `xorriso : FAILURE : Cannot find path '/efi.img' in loaded ISO image`

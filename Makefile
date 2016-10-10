export HOST:=i686-elf
export ARCH:=$(shell if echo $(HOST) | grep -Eq 'i[[:digit:]]86-'; then \
  echo i386 ; \
else \
  echo $(HOST) | grep -Eo '^[[:alnum:]_]*' ; \
fi)
export HOSTARCH:=$(ARCH)

SYSTEM_HEADER_PROJECTS:=libc kernel
PROJECTS:=$(SYSTEM_HEADER_PROJECTS) test

export MAKE:=$(MAKE:-make)

export AR:=$(HOST)-ar
export AS:=$(HOST)-as
export CC:=$(HOST)-gcc
export CXX:=$(HOST)-g++
export NAS:=nasm

export PREFIX:=/usr
export EXEC_PREFIX:=$(PREFIX)
export BOOTDIR:=/boot
export LIBDIR:=$(EXEC_PREFIX)/lib
export INCLUDEDIR:=$(PREFIX)/include

export CFLAGS:=-O0 -g
export CPPFLAGS:=-fno-rtti -fno-exceptions
export NASFLAGS:=-felf32

export CC:=$(CC) --sysroot=$(PWD)/sysroot
export CC:=$(shell if echo $(HOST) | grep -Eq -- '-elf($$|-)'; then \
	echo "$(CC) -isystem=$(INCLUDEDIR)" ; \
fi)
export CXX:=$(CXX) --sysroot=$(PWD)/sysroot
export CXX:=$(shell if echo $(HOST) | grep -Eq -- '-elf($$|-)'; then \
	echo "$(CXX) -isystem=$(INCLUDEDIR)" ; \
fi)
export CPPFLAGS:=$(shell if echo $(HOST) | grep -Eq -- '-elf($$|-)'; then \
	echo "$(CPPFLAGS) -D__HAS_NO_CRT_INIT" ; \
fi)
export DESTDIR:=$(PWD)/sysroot

.PHONY: all iso run clean projects prep sysroot headers lib

all: iso

iso: projects sysroot
	@ echo "Creating ISO"
	@ echo "Building initrd"
	@ tar --create --file isodir/boot/initrd.tar --directory=sysroot $(shell ls sysroot)
	@ cp grub/grub.cfg isodir/boot/grub/grub.cfg
	@ echo "Generating ISO"
	@ grub-mkrescue -o chronos.iso isodir > /dev/null 2>&1
	@ echo "Done\n"

sysroot:
	@ echo "Preparing $@"
	@ mkdir -p isodir
	@ mkdir -p isodir/boot
	@ mkdir -p isodir/boot/grub
	@ cp -rf sysroot/* isodir/
	@ cp sysroot/boot/chronos.kernel isodir/boot/chronos.kernel
	@ echo "Done\n"

projects: headers
	@ echo "Setting up $@"
	@ for PROJECT in $(PROJECTS); do \
		echo "\t→$$PROJECT" ; \
		make -s -C $$PROJECT install ; \
	done
	@ echo "Done\n"

lib: headers
	make -s -C libc

headers:
	@echo "Setting up $@"
	@ mkdir -p sysroot
	@ for PROJECT in $(SYSTEM_HEADER_PROJECTS); do \
		echo "\t→$$PROJECT" ; \
		make -s -C $$PROJECT install-headers ; \
	done
	@ echo "Done\n"

run: all qemu

run-curses: all qemu-curses

qemu-curses:
	qemu-system-$(ARCH) -cdrom chronos.iso -curses

qemu:
	qemu-system-$(ARCH) -cdrom chronos.iso -serial stdio

clean:
	@ echo "Cleaning"
	@ for PROJECT in $(PROJECTS); do \
		echo "\t→$$PROJECT" ; \
		make -s -C $$PROJECT clean ; \
	done
	@ echo "\t→root project"
	@ rm -rf sysroot
	@ rm -rf isodir
	@ rm -rf chronos.iso
	@ echo "Done\n"

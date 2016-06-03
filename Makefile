export HOST:=i686-elf
export ARCH:=$(shell if echo $(HOST) | grep -Eq 'i[[:digit:]]86-'; then \
  echo i386 ; \
else \
  echo $(HOST) | grep -Eo '^[[:alnum:]_]*' ; \
fi)
export HOSTARCH:=$(ARCH)

SYSTEM_HEADER_PROJECTS:=libc kernel
PROJECTS:=$(SYSTEM_HEADER_PROJECTS)

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
export CPPFLAGS:=
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

.PHONY: all iso run clean headers

all: iso

iso: kernel
	mkdir -p isodir
	mkdir -p isodir/boot
	mkdir -p isodir/boot/grub
	cp sysroot/boot/chronos.kernel isodir/boot/chronos.kernel
	tar --verbose --create --file isodir/boot/chronos.initrd --directory=sysroot $(shell ls sysroot | grep -v boot)
	cp grub/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o chronos.iso isodir

kernel: headers
	for PROJECT in $(PROJECTS); do \
		make -C $$PROJECT install ; \
	done

headers:
	mkdir -p sysroot
	for PROJECT in $(SYSTEM_HEADER_PROJECTS); do \
		make -C $$PROJECT install-headers ; \
	done

run: all qemu

qemu:
	qemu-system-$(ARCH) -cdrom chronos.iso -serial stdio

clean:
	for PROJECT in $(PROJECTS); do \
		make -C $$PROJECT clean ; \
	done
	rm -rfv sysroot
	rm -rfv isodir
	rm -rfv chronos.iso

CFLAGS?=-O0 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=-L=/usr/lib

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin -Werror -Wall -Wextra -Wno-format
CPPFLAGS:=$(CPPFLAGS) -D__is_chronos_kernel -Iinclude -Wno-pointer-arith -Wno-type-limits
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR:=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

OBJS:=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \
kernel/panic.o \
kernel/icxxabi.o \
kernel/memory/heap.o \
kernel/memory/ramdisk.o \
kernel/hardware/device.o \
kernel/hardware/tty.o \
kernel/graphics/video.o \
kernel/graphics/window.o \
kernel/graphics/context.o \
kernel/graphics/desktop.o \
kernel/graphics/rect.o \
kernel/fs/tar.o \

CRTI_OBJ:=$(ARCHDIR)/asm/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=$(ARCHDIR)/asm/crtn.o

ALL_OBJS:=\
$(CRTI_OBJ) \
$(OBJS) \
$(CRTN_OBJ)

OBJ_LINK_LIST:=\
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(OBJS) \
$(CRTEND_OBJ) \
$(CRTN_OBJ)

all: chronos.kernel

.PHONY: all clean install install-headers install-kernel

chronos.kernel: $(OBJ_LINK_LIST) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ -std=gnu++11 $(CFLAGS) $(CPPFLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.o: %.asm
	$(NAS) $(NASFLAGS) $< -o $@

clean:
	rm -f chronos.kernel $(OBJS) $(ALL_OBJS) *.o */*.o */*/*.o

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv include $(DESTDIR)$(INCLUDEDIR)

install-kernel: chronos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp chronos.kernel $(DESTDIR)$(BOOTDIR)

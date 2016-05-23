HOST:=$(shell ./default-host.sh)
ARCH:=$(shell ./target-arch.sh $(HOST))

.PHONY: all run clean

all: iso

iso:
	./iso.sh

run: all
	qemu-system-$(ARCH) -cdrom chronos.iso

clean:
	./clean.sh

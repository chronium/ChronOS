NASFLAGS=-fbin
NAS=nasm

all: test.bin

%.bin: %.asm
	$(NAS) $(NASFLAGS) $< -o $@

clean:
	@ rm -f *.bin

install: all
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp test.bin $(DESTDIR)$(BOOTDIR)

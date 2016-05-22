#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/chronos.kernel isodir/boot/chronos.kernel
tar --verbose --create --file isodir/boot/chronos.initrd --directory=sysroot $(ls sysroot | grep -v boot)
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "ChronOS" {
  multiboot /boot/chronos.kernel
  module /boot/chronos.initrd
}
EOF
grub-mkrescue -o chronos.iso isodir

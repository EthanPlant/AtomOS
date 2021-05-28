#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/atomos.kernel isodir/boot/atomos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "atomos" {
    multiboot /boot/atomos.kernel
}
EOF
grub-mkrescue -o atomos.iso isodir
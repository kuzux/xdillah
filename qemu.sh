#!/bin/sh
set -e
. ./build.sh
 
qemu-system-$(./target-triplet-to-arch.sh $HOST) -kernel sysroot/boot/xdillah.kernel -initrd initrd.txt -serial stdio

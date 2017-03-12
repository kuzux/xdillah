#!/bin/sh
set -e
. ./build.sh
. ./initrd.sh
 
qemu-system-$(./target-triplet-to-arch.sh $HOST) -kernel sysroot/boot/xdillah.kernel -initrd initrd.img -serial stdio -s -S

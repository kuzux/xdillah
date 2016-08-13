#!/bin/sh
set -e
. ./build.sh
 
qemu-system-$(./target-triplet-to-arch.sh $HOST) -kernel sysroot/boot/xdillah.kernel -serial stdio

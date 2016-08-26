#!/bin/sh
set -e
. ./build.sh
 
for PROJECT in $INITRD_PROJECTS; do
  cp "$PROJECT/$PROJECT" initrd_contents/
done

generate_initrd/generate_initrd initrd_contents/*

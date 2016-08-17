#!/bin/sh
set -e
. ./build.sh

generate_initrd/generate_initrd initrd_contents/*

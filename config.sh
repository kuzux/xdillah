SYSTEM_HEADER_PROJECTS="libc kernel generate_initrd"
PROJECTS="libc kernel generate_initrd"
 
export MAKE=${MAKE:-make}
export HOST=${HOST:-$(./default-host.sh)}
 
export AR=~/opt/cross/bin/${HOST}-ar
export AS=~/opt/cross/bin/${HOST}-as
export CC=~/opt/cross/bin/${HOST}-gcc
 
export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include
export BINDIR=$PREFIX/bin

export CFLAGS='-O2 -g'
export CPPFLAGS=''
 
# Configure the cross-compiler to use the desired system root.
export CC="$CC --sysroot=$PWD/sysroot"
 
# Work around that the -elf gcc targets doesn't have a system include directory
# because configure received --without-headers rather than --with-sysroot.
if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
  export CC="$CC -isystem=$INCLUDEDIR"
fi

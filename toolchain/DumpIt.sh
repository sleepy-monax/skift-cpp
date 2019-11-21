#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PREFIX="$DIR/local"

if [ -e "$PREFIX/build-ok" ]; then
    echo "The toolchain is build and should be ready :)"
fi

echo "Uname:"
uname -a
echo --------------------------------------------------------------------------------

echo "Bash:"
bash --version
echo --------------------------------------------------------------------------------

echo "GCC:"
gcc --version
echo --------------------------------------------------------------------------------

echo "LD:"
ld --version
echo --------------------------------------------------------------------------------

echo "Nasm:"
nasm --version
echo --------------------------------------------------------------------------------

echo "Make:"
make --version

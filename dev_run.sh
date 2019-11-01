#!/bin/bash
make -C ./src distclean
make -C ./src
make -C ./src clean

./src/main.exe "$@"
#!/bin/bash
./scripts/clean_all.sh
make -C ./src DEBUG=1

./src/main.exe "$@"
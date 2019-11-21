#!/bin/bash
./scripts/clean_all.sh
make -C ./src DEBUG=1

mkdir -p log

valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --verbose \
    --show-reachable=no \
    --leak-resolution=high \
    --log-file=log/valgrind-report.txt \
    ./src/main.exe "$@"

echo -e "\n-----------------------------------------------\n- Note that SDL can cause leaks, check origin -\n-----------------------------------------------\n" >> log/valgrind-report.txt
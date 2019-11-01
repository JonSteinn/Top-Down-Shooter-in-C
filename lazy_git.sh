#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Two parameters needed!"
    exit 1
fi

make -C ./src distclean
make -C ./src clean

git add .
git commit -m "$2"
git push "https://jonsteinn:$1@github.com/jonsteinn/c-game.git"
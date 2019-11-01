#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Two parameters needed!"
    exit 1
fi

make -C ./src distclean
make -C ./src clean

git add .
git commit -m "$1"
git config user.email "jonsteinn@gmail.com"
git config user.name "jonstenn"
git config user.password "$2"
git push origin master

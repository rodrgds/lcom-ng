#!/bin/sh

DIRS="lab1 lab2 lab3 lab4 lab5 teclas"

for dir in $DIRS; do
    echo "$dir"
    cd "$dir" || { echo "Failed to enter $dir"; exit 1; }
    make clean && make
    cd ..
done

cd teclas
make clean && make proj
cd ..
lcom_run teclas/proj

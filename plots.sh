#!/bin/bash

if [ ! -d plots ]; then 
    mkdir plots
fi

for file in $@; do
    python3 plots.py $file
done
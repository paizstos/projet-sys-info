#!/bin/bash

if [ $# -lt 1 ]; then
    echo "One argument needed. $# given." > /dev/stderr
    exit 1
fi

if [ ! -d benchmarks ]; then
    mkdir benchmarks
fi

for file in $@; do

    bench_path=$(basename ${file})

    out_path=$(pwd)/benchmarks/${bench_path}.csv

    touch $out_path
    echo "nthreads,seconds" > $out_path

    array=("2" "4" "8" "16" "32" "64")

    if [ ${file} = "Philosophe" ]; then
        for NTHREADS in ${array[@]}; do
            for I in {1..5}; do
                TIME=`/usr/bin/time -a -f %e -o /dev/stdout ./${file} ${NTHREADS}`
                echo ${NTHREADS},${TIME} >> $out_path
            done
        done
    else
        for NTHREADS in ${array[@]}; do
            NTHREADSDIV=${NTHREADS}/2
            for I in {1..5}; do
                TIME=`/usr/bin/time -a -f %e -o /dev/stdout ./${file} ${NTHREADSDIV} ${NTHREADSDIV}`
                echo ${NTHREADS},${TIME} >> $out_path
            done
        done
    fi

done
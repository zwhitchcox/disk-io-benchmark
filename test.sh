#!/bin/bash

set -e

file="${1:-testing.txt}"

make benchmark
echo benchmarking $file...
case $file in
  "/dev/sd"*)
    sudo ./build/benchmark $file
    ;;
  *)
    rm -f $file
    ./build/benchmark $file
esac
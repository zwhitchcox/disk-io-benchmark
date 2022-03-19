#!/bin/bash

set -e

file="${1:-testing.txt}"

make benchmark
case $file in
  "/dev/sd"*)
    sudo ./build/benchmark $file
    ;;
  *)
    rm -f $file
    ./build/benchmark $file
esac
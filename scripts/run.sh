#!/bin/bash

set -ex

JOBS=2

test_base=$(dirname `dirname $(realpath "${BASH_SOURCE[-1]}")`)/files
default_files=("$test_base/rand_1gb.txt" "$test_base/rand_1gbo.txt")

rm -f ${default_files[1]}
if [ "$?" != "0" ]; then exit; fi

size=1gb

passthrough=()

while getopts "itd" arg; do
  case $arg in
    i)
      interactive=true
      ;;
    t)
      test=true
      ;;
    d)
      passthrough+=(-d)
      ;;
  esac
done

if [ $test == true ]; then
  make benchmark
  cmd="./build/benchmark"
else
  make benchmark_debug
  cmd=(gdb)
  if [ $interactive != true ]; then
    cmd+=(-batch)
  fi
  cmd+=(-q)
  # cmd+=(-ex "set print thread-events off")
  cmd+=(-ex run --args ./build/benchmark_debug)
fi


set -x
# commands passed after -- will override defaults set here
"${cmd[@]}" -j $JOBS ${passthrough[@]} ${@:$OPTIND} "${default_files[@]}"
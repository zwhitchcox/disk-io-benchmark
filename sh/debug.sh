#!/bin/bash

set -e

test_base=$(dirname `dirname $(realpath "${BASH_SOURCE[-1]}")`)/test-data
default_files=("$test_base/test_random_input.txt" "$test_base/test_random_output.txt")

rm -f ${default_files[1]}
if [ "$?" != "0" ]; then exit; fi

while getopts ":i" arg; do
  case $arg in
    i)
      interactive=true
      ;;
  esac
done

case $0 in
  *test.sh)
    make fio
    cmd="./build/fio"
  ;;
  *debug.sh)
    make fio_debug
    cmd=(gdb)
    if [ "$interactive" != "true" ]; then
      cmd+=(-batch)
    fi
    cmd+=(-q -ex "set print thread-events off" -ex run --args ./build/fio_debug)
  ;;
esac

set -x
# commands passed after -- will override defaults set here
"${cmd[@]}" -j $(($(nproc)/2)) ${@:$OPTIND} "${default_files[@]}"
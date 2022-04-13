#!/bin/bash

test_base=$(dirname `dirname $(realpath "${BASH_SOURCE[-1]}")`)/test-data
default_files=("$test_base/test_random_input.txt" "$test_base/test_random_output.txt")

rm -f ${default_files[1]}
if [ "$?" != "0" ]; then exit; fi

case $0 in
  *test.sh)
    make fio
    cmd="./build/fio"
  ;;
  *debug.sh)
    make fio_debug
    cmd=(gdb -q -batch -ex "set print thread-events off" -ex run --args ./build/fio_debug)
  ;;
esac

set -x
# commands passed after -- will override defaults set here
"${cmd[@]}" -j $(nproc) $* "${default_files[@]}"

#!/bin/bash

files=()
args=()
while [ -n "$1" ]; do
  case "$1" in
  -*)
    args+=($1)
    ;;
  *)
    files+=($1)
    ;;
  esac
  shift
done

test_base=$(dirname `dirname $(realpath "${BASH_SOURCE[-1]}")`)/test-data
if [ ${#files[@]} == 0 ]; then
  files+=("$test_base/test_random_input.txt" "$test_base/test_random_output.txt")
fi

rm -f ${files[1]}
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
"${cmd[@]}" -j $(nproc) $* "${files[@]}"

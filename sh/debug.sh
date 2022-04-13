#!/bin/bash

while getopts "h" arg; do
  case $arg in
    h)
      echo "Usage (debug|test).sh [input_file] [output_file] [-- passthrough_cli_options]"
  esac
done

test_base=$(dirname `dirname $(realpath "${BASH_SOURCE[-1]}")`)/test-data
input_file=${@:$OPTIND:1}
input_file=${input_file:-"$test_base/test_random_input.txt"}
output_file=${@:$OPTIND+1:1}
output_file=${output_file:-"$test_base/test_random_output.txt"}
file_args="$input_file $output_file"


rm -f $test_output_file
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

# commands passed after -- will override defaults set here
"${cmd[@]}" -j $(nproc) $* $file_args

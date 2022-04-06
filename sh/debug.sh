#!/bin/bash
# echo WITHOUT VERIFICATION
# ./build/fio_no_verify ./testing.txt

# echo
# echo WITH VERIFICATION
# ./build/fio_verify ./testing.txt

# get a random string of data
PAGE_SIZE=512
random_string="$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | head --bytes $PAGE_SIZE)"
test_input_file=${1:-"./test-data/test_input.txt"}
test_output_file=${2:-"./test-data/test_output.txt"}
GB=$((1024*1024*1024))
pages=$((10*$GB/$PAGE_SIZE))
if ! test -f $test_input_file; then
  for x in $(seq 1 $pages); do
    echo $random_string >> $test_input_file
  done
fi
rm -f $test_output_file
make fio_debug
./build/fio_debug $test_input_file $test_output_file
#!/bin/bash

# for testing speed
TEST_RANDOM_INPUT_PATH=test-data/test_random_input.txt
# helpful for debugging incorrect writes
TEST_SEQ_INPUT_PATH=test-data/test_seq_input.txt

mkdir -p test-data

if ! test -f $TEST_RANDOM_INPUT_PATH; then
  echo creating $TEST_RANDOM_INPUT_PATH
  echo This will take a while...
  cat /dev/random | tr -dc 'a-zA-Z0-9' | head --bytes $((10*1024**3)) >> $TEST_RANDOM_INPUT_PATH
fi

if ! test -f $test_seq_file; then
  echo Creating $test_seq_file
  for i in $(seq 1 $((100))); do
    echo $i >> $test_seq_file;
  done
fi
#!/bin/bash

# for testing speed
TEST_RANDOM_PATH=test-data/test_random_input.txt
# helpful for debugging incorrect writes
TEST_SEQ_PATH=test-data/test_seq_input.txt

mkdir -p test-data

if ! test -f $TEST_RANDOM_PATH; then
  echo creating $TEST_RANDOM_PATH
  echo This will take a while...
  for i in `seq 1 4`; do
    cat /dev/random | tr -dc 'a-zA-Z0-9' | head --bytes $((2*1024**3)) >> $TEST_RANDOM_PATH_tmp &
  done
fi
mv $TEST_RANDOM_PATH_tmp $TEST_RANDOM_PATH

if ! test -f $TEST_SEQ_PATH; then
  echo Creating $TEST_SEQ_PATH
  for i in $(seq 1 $((1024**2))); do
    echo $i >> $TEST_SEQ_PATH;
  done
fi
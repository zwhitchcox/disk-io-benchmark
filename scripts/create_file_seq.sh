#!/bin/bash

# for testing speed
PATH_BASE=files/rand
gb=8
# helpful for debugging incorrect writes
SEQ_PATH=files/seq1000.txt

mkdir -p files

numproc=$(($(nproc)/2))

if ! test -f $TEST_SEQ_PATH; then
  echo Creating $TEST_SEQ_PATH
  for i in $(seq 1 $((1024**2))); do
    echo $i >> $TEST_SEQ_PATH;
  done
fi
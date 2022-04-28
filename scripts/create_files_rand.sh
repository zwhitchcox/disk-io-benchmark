#!/bin/bash

# create file for testing speed


FILES_DIR=./files
PATH_BASE=$FILES_DIR/rand
GB=$((1024**3))
NUM_PROC=$(($(nproc)/2))

mkdir -p $FILES_DIR
size=8

# echo creating $sizegb random file.
# echo This could take a while...
# for i in `seq 1 $NUM_PROC`; do
#   cat /dev/random |
#   tr -dc 'a-zA-Z0-9' |
#   head --bytes $(($size/$NUM_PROC*$GB)) >> $BASE_PATH_$sizegb.txt &
# done

function filename() {
  echo "${PATH_BASE}_${1}gb.txt"
}

while [ $size -gt 1 ]; do
  new_size=$(($size/2))
  echo Creating `filename $new_size`
  cp `filename $size`  `filename $new_size`
  truncate --size $((new_size*$GB)) `filename $new_size`
  size=$new_size
done
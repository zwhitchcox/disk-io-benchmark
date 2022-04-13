mkdir -p test-data

test_input_file=test-data/test_input.txt
test_seq_file=test-data/test_input_seq.txt

if ! test -f $test_input_file; then
  echo creating $test_input_file
  for x in $(seq 1 $pages); do
    echo $random_string >> $test_input_file
  done
fi

if ! test -f $test_seq_file; then
  echo creating $test_seq_file
  for i in $(seq 1 $((100))); do
    echo $i >> $test_seq_file;
  done
fi
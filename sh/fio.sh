# fio --filename=test.txt --direct=1 --size=1G --rw=randread --bs=4k --ioengine=libaio --iodepth=256 --runtime=120 --numjobs=$(nproc) --time_based --group_reporting --name=iops-test-job --eta-newline=1 --readonly
fio --filename=./test.txt \
  --rw=rw \
  --direct=1 \
  --bs=64k \
  --ioengine=libaio \
  --iodepth=64 \
  --group_reporting \
  --name=throughput-test-job \
  --eta-newline=1 \
  --size=10g \
  --io_size=10g \
  --enable-gfio \
  --overwrite=1
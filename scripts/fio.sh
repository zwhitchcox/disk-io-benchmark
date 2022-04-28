# fio --filename=test.txt --direct=1 --size=1G --rw=randread --bs=4k --ioengine=libaio --iodepth=256 --runtime=120 --numjobs=$(nproc) --time_based --group_reporting --name=iops-test-job --eta-newline=1 --readonly
# fio --filename=${1:-./test-data/test.txt} \
#   --rw=rw \
#   --bs=4k \
#   --ioengine=libaio \
#   --iodepth=64 \
#   --group_reporting \
#   --name=throughput-test-job \
#   --eta-newline=1 \
#   --size=1g \
#   --io_size=10g \
#   --overwrite=1
  # --direct=1

  sudo fio --filename=/dev/sda --rw=write --bs=64k --ioengine=libaio --iodepth=64 --numjobs=4 --time_based -t group_reporting --eta-newline=1 --size=10g
#  sudo fio --filename=/dev/sda --rw=write --bs=64k --ioengine=libaio --iodepth=64 --numjobs=4 --time_based --group_reporting --name=throughput-test-job --verify=xxhash --eta-newline=1 --size=10g --io_size 10g

make clean
make
sudo insmod pxt4.ko
mkfs -t ext4 /dev/sdb
sudo mount -t pxt4 /dev/sdb /mnt/test
sudo rm -rf /mnt/test/*
sudo fio zeo.fio
sudo umount /mnt/test
sudo rmmod pxt4
dmesg

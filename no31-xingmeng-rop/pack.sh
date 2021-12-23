#!/bin/sh


mkdir /tmp/rootfs_tmp
cp -r ../_install/* /tmp/rootfs_tmp
cp -r ../env/bin_root/* /tmp/rootfs_tmp
cp -r rootfs/* /tmp/rootfs_tmp

gcc -g -static exp.c -o exp
cp init /tmp/rootfs_tmp/
cp *.ko /tmp/rootfs_tmp/
cp exp /tmp/rootfs_tmp/

chmod +x /tmp/rootfs_tmp/init
chmod g-w -R /tmp/rootfs_tmp/
chmod o-w -R /tmp/rootfs_tmp/
sudo chown -R root /tmp/rootfs_tmp/
sudo chgrp -R root /tmp/rootfs_tmp/
sudo chmod u+s /tmp/rootfs_tmp/bin/busybox
sudo chown -R 1000:1000 /tmp/rootfs_tmp/home/* 
#sudo chmod 644 rootfs_tmp/etc/passwd
#sudo chmod 644 rootfs_tmp/etc/shadow

ndir=`pwd`
cd /tmp/rootfs_tmp
find . | cpio -o -H newc > $ndir/rootfs.cpio
cd $ndor

sudo rm -rf /tmp/rootfs_tmp

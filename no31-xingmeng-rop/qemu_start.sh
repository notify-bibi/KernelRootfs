qemu-system-x86_64 \
       -cpu kvm64,-smap,-smep -m 1024 -smp cores=1,threads=1 \
       -kernel bzImage \
       -initrd rootfs.cpio \
       -nographic \
       -monitor none \
       -append "nokaslr console=ttyS0 oops=panic panic=1 quiet" \
       -s



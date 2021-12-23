#!/bin/sh

# dont remote add any
exec gdb -q \
    -ex "file vmlinux" \
    -ex "source /home/u18/source/linux-4.4.72/vmlinux-gdb.py" \
    -ex "add-auto-load-safe-path /home/u18/source/linux-4.4.72/" \
    -ex "add-auto-load-safe-path /home/u18/env/" \
    -ex "add-symbol-file test_src.ko 0xffffffffc0000000" \
    -ex "b test_ioctl" \
    -ex "lx-symbols" \
    -ex "" \
    $@

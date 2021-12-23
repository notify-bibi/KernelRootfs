#!/bin/sh

case "$@" in
    *"--interpreter=mi"*) exec socat pty,link=/tmp/ptsgdb,raw,echo=0 - ; exit $? ;;
    *) exec gdb $@ ; exit $? ;;
esac



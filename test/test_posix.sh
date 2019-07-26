#!/bin/bash
export CRAYPE_LINK_TYPE=dynamic
cc -o posix_rdwt posix_rdwt.c
GOTCHAPOSIX=../etalumis_wrapper/lwrapposix_rdwt.so
GOTCHALIB=${GOTCHA}/lib64/libgotcha.so
if test -f "$GOTCHALIB"; then
 LD_PRELOAD=$GOTCHAPOSIX:$GOTCHALIB ./posix_rdwt
fi
rm foo.txt

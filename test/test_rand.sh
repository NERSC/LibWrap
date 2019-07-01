#!/bin/bash
export CRAYPE_LINK_TYPE=dynamic
cc -o myrand myrand.c
GOTCHAPOSIX=../mywrapper/lwraprand.so
GOTCHALIB=${GOTCHA}/lib64/libgotcha.so
if test -f "$GOTCHALIB"; then
 LD_PRELOAD=$GOTCHAPOSIX:$GOTCHALIB ./myrand
fi

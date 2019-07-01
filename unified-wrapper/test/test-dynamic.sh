#!/bin/bash
module load cray-hdf5
export CRAYPE_LINK_TYPE=dynamic
echo "============"
export GOTCHA=/global/homes/a/aalmamun/gotcha/install

GOTCHAH5=../wrapper/libgotchah5.so
LIB_DIR=/global/homes/a/aalmamun/lbl-wrapper/dynamic-wrapper
GOTCHADIR=${GOTCHA}/lib64
GOTCHALIB=${GOTCHA}/lib64/libgotcha.so
if test -f "$GOTCHALIB"; then
 echo "Testing apps with COMMON"
 echo "============"
 cc  linux.c -o linux #-Wl,--wrap=open -L${LIB_DIR} -L${GOTCHADIR} -lgotcha -lgotchah5 -ltest -o linux 
 cc h5_crtdat.c -o h5_crtdat 
#cc wrapper_hdf5.c -o wrapper_hdf5
 echo $GOTCHAH5
 echo $GOTCHALIB
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./h5_crtdat
 ldd h5_crtdat 
 echo "============"
 echo "Done"
fi

#!/bin/bash
module load cray-hdf5
export CRAYPE_LINK_TYPE=dynamic
echo "============"
#export GOTCHA=/global/homes/a/aalmamun/gotcha/install

GOTCHAH5=../mywrapper/lwraphdf5.so
GOTCHALIB=${GOTCHA}/lib64/libgotcha.so
if test -f "$GOTCHALIB"; then
 echo "Testing apps with COMMON"
 echo "============"
 cc h5_crtdat.c -o h5_crtdat 
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./h5_crtdat
 echo "============"
 echo "Done"
fi


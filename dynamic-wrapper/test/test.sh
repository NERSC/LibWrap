#!/bin/bash
module load cray-hdf5
export CRAYPE_LINK_TYPE=dynamic
echo "============"
echo "Compiling apps"
echo "============"
#cc -o h5_crtdat h5_crtdat.c
cc -o linux linux.c

echo "Done"
echo "============"
echo "Testing apps without GOTCHA"
echo "============"
./linux
./h5_crtdat

echo "Done"
echo "============"
export GOTCHA=/global/homes/a/aalmamun/gotcha/install
GOTCHAH5=../libgotchah5.so
GOTCHALIB=${GOTCHA}/lib64/libgotcha.so
if test -f "$GOTCHALIB"; then
 echo "Testing apps with GOTCHA"
 echo "============"
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./linux
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./h5_crtdat
 echo "============"
 echo "Done"
fi


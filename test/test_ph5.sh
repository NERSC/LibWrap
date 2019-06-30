#!/bin/bash
module load cray-hdf5-parallel
export CRAYPE_LINK_TYPE=dynamic
echo "============"
echo "Compiling HDF5 apps"
echo "============"
cc -o ph5example ph5example.c
echo "Done"
echo "============"
echo "Testing HDF5 apps without GOTCHA"
echo "============"
./ph5example
echo "Done"
echo "============"
GOTCHAH5=../mywrapper/lwraphdf5.so
GOTCHALIB=${GOTCHA}/lib64/libgotcha.so
if test -f "$GOTCHALIB"; then
 echo "Testing HDF5 apps with GOTCHA"
 echo "============"
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./ph5example
 echo "============"
 echo "Done"
fi


#!/bin/bash
module load cray-hdf5-parallel
export CRAYPE_LINK_TYPE=dynamic
echo "============"
echo "Compiling HDF5 apps"
echo "============"
cc -o h5p_crtdat h5p_crtdat.c
#cc -o h5_rdwt h5_rdwt.c
echo "Done"
echo "============"
echo "Testing HDF5 apps without GOTCHA"
echo "============"
./h5p_crtdat
#./h5_rdwt
echo "Done"
echo "============"
GOTCHAH5=../lgotchah5.so
GOTCHALIB=${GOTCHA}/lib64/libgotcha.so
if test -f "$GOTCHALIB"; then
 echo "Testing HDF5 apps with GOTCHA"
 echo "============"
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./h5p_crtdat
 #LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./h5_rdwt
 echo "============"
 echo "Done"
fi


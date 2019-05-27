#!/bin/bash
module load cray-hdf5


export CRAYPE_LINK_TYPE=static
echo "============"
echo "Compiling HDF5 apps"
echo "============"


cc -o h5_crtdat h5_crtdat.c
cc -o h5_rdwt h5_rdwt.c
cc -o h5_mount h5_mount.c
echo "Done"
echo "============"
echo "Testing HDF5 apps without GOTCHA"
echo "============"
./h5_crtdat
./h5_rdwt
./h5_mount
echo "Done"
echo "============"

GOTCHAH5=../lgotchah5.a
GOTCHALIB=${GOTCHA}/lib64/libgotcha.a
if test -f "$GOTCHALIB"; then
 echo "Testing HDF5 apps with GOTCHA"
 echo "============"
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./h5_crtdat
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./h5_rdwt
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./h5_mount
 echo "============"
 echo "Done"
fi


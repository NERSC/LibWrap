#!/bin/bash
module load cray-hdf5
export CRAYPE_LINK_TYPE=dynamic
echo "============"
echo "Compiling HDF5 apps"
echo "============"
cc -o h5_crtdat h5_crtdat.c
cc -o h5_rdwt h5_rdwt.c
echo "Done"
echo "============"
echo "Testing HDF5 apps without GOTCHA"
echo "============"
./h5_crtdat
./h5_rdwt
echo "Done"
echo "============"
GOTCHAH5=/global/homes/j/jialin/gotcha/gotcha_hdf5/lgotchah5.so
GOTCHA=/global/homes/j/jialin/gotcha/gotcha/install/lib64/libgotcha.so
if test -f "$GOTCHA"; then
 echo "Testing HDF5 apps with GOTCHA"
 echo "============"
 LD_PRELOAD=$GOTCHAH5:$GOTCHA ./h5_crtdat
 LD_PRELOAD=$GOTCHAH5:$GOTCHA ./h5_rdwt
 echo "============"
 echo "Done"
fi


#!/bin/bash
#module load cray-hdf5
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
GOTCHAH5=../mywrapper/libwraphdf5.so
GOTCHALIB=${GOTCHA}/lib64/libgotcha.so
#HDF5=${HOME}/LibWrap-Project/LibWrap/dockers/shared/libhdf5_intel.so
if test -f "$GOTCHALIB"; then
 echo "Testing HDF5 apps with GOTCHA"
 echo "============"
# LD_PRELOAD=$GOTCHAH5:$GOTCHALIB:$HDF5 ./h5_crtdat
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./h5_rdwt
 echo "============"
 echo "Done"
fi

echo "Testing without gotcha shared lib"
LD_PRELOAD=$GOTCHAH5 ./h5_rdwt
echo "Done"

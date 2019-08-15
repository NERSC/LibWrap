#!/bin/bash
#module load cray-hdf5
export CRAYPE_LINK_TYPE=dynamic
echo "============"
echo "Compiling HDF5 apps"
echo "============"
#module swap PrgEnv-intel PrgEnv-gnu
cc -o h5_crtdat h5_crtdat.c
cc -o h5_rdwt h5_rdwt.c
echo "Done"
echo "============"
echo "Testing HDF5 apps without Reflector"
echo "============"
./h5_crtdat
./h5_rdwt
echo "Done"
echo "============"
GOTCHAH5=../mywrapper/libwraphdf5.so

echo "Testing with Reflector"
LD_PRELOAD=$GOTCHAH5 ./h5_rdwt
LD_PRELOAD=$GOTCHAH5 ./h5_crtdat
echo "Done"

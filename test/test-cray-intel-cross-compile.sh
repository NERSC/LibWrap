#!/bin/bash
#This script demonstrates how to use intel compiled reflector library with user app compiled in cray compiler

module load cray-hdf5
export CRAYPE_LINK_TYPE=dynamic
echo "============"
echo "Compiling HDF5 apps with CRAY compiler"
echo "============"
module swap PrgEnv-intel PrgEnv-cray
cc -o h5_crtdat h5_crtdat.c
echo "============"
echo "Done"
echo "============"

echo "Testing CRAY compiled user app with intel compiled reflector"

GOTCHAH5=../mywrapper/libwraphdf5.so
LIBIMF=/opt/intel/compilers_and_libraries_2019.3.199/linux/compiler/lib/intel64/libimf.so
LIBSVML=/opt/intel/compilers_and_libraries_2019.3.199/linux/compiler/lib/intel64/libsvml.so
LIBIRNG=/opt/intel/compilers_and_libraries_2019.3.199/linux/compiler/lib/intel64/libirng.so
LIBINTLC=/opt/intel/compilers_and_libraries_2019.3.199/linux/compiler/lib/intel64/libintlc.so.5
export LD_LIBRARY_PATH=/opt/intel/compilers_and_libraries_2019.3.199/linux/compiler/lib/intel64/:$LD_LIBRARY_PATH
LD_PRELOAD=$GOTCHAH5:$LIBIMF:$LIBSVML:$LIBIRNG ./h5_crtdat
#echo $LD_LIBRARY_PATH
echo "================="
echo "Done"
echo "================="

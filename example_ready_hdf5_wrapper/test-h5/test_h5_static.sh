#!/bin/bash
#module load cray-hdf5
export CRAYPE_LINK_TYPE=static
#	$(shell bash static-object-generator.sh)
#module swap PrgEnv-intel PrgEnv-gnu
wrapper_libdir=../mywrapper
cc h5_crtdat.c -o  output  -Wl,--wrap=H5Fcreate -Wl,--wrap=H5Fopen -Wl,--wrap=H5Fclose  -L${wrapper_libdir} -lwraphdf5  
#=======
#cc h5_crtdat.c -o  output -Wl,--wrap=H5Fcreate -Wl,--wrap=H5Fopen -Wl,--wrap=H5Fclose  -L${wrapper_libdir} -lwraphdf5 -I /LibWrap/dockers/shared/include 
./output

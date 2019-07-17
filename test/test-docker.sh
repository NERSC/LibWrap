#!/bin/bash
export LD_LIBRARY_PATH=/LibWrap/dockers/shared
cc -o h5_crtdat h5_crtdat.c  -I $(HOME)/LibWrap-Project/LibWrap/dockers/shared/include
cc -o h5_rdwt h5_rdwt.c -I /LibWrap/dockers/shared/include
GOTCHAH5=../mywrapper/libwraphdf5.so
GOTCHALIB=/LibWrap/dockers/shared/lib64/libgotcha.so
if test -f "$GOTCHALIB"; then
 echo "Testing HDF5 apps with GOTCHA"
 echo "============"
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./h5_crtdat
 LD_PRELOAD=$GOTCHAH5:$GOTCHALIB ./h5_rdwt
 echo "============"
 echo "Done"
fi


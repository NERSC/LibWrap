#module load gnu-wrapper
wrapper_libdir=../wrapper
cc  h5_crtdat.c -o output -Wl,--wrap=H5Fcreate  -L${wrapper_libdir} -lgotchah5 # -o  output #-lwrapper
./output

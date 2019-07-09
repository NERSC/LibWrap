#module load gnu-wrapper
wrapper_libdir=../mywrapper
cc  h5_crtdat.c -o output -Wl,--wrap=H5Fcreate  -L${wrapper_libdir} -lwraphdf5 # -o  output #-lwrapper
./output

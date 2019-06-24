#module load gnu-wrapper
wrapper_libdir=../wrapper
cc -g linux.c -o output -Wl,--wrap=open  -L${wrapper_libdir} -lgotchah5 # -o  output #-lwrapper
./output

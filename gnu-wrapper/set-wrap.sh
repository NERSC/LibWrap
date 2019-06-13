#!/bin/bash
prefix=$PWD
git clone https://github.com/llnl/gotcha
cd gotcha
mkdir build install
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=../install
make
make install

cd $prefix #../..

#prefix=$PWD

export GOTCHA=$prefix/gotcha/install
echo $GOTCHA

git clone https://github.com/NERSC/gotcha-io.git
cd gotcha-io
module load cray-hdf5
make

cd $prefix


mkdir all_wrap
cp gotcha-io/lgotchah5.so all_wrap/
cd all_wrap
mv lgotchah5.so libgotchah5.so
cd $prefix
#export LD_PRELOAD=$prefix/all_wrap/libgotchah5.so:$GOTCHA/lib64/libgotcha.so
cp $GOTCHA/lib64/libgotcha.so all_wrap/

#git clone static wrapper
cd static-wrapper

cc -c wrappe.c -o wrappe.o
ar rcs libwrapper.a wrappe.o
cp libwrapper.a ../all_wrap/


 

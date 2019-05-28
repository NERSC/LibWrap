## Gotcha-IO
* IO analysis with Gotcha for dynamically linked apps
* Automatic tracing with light overhead at scale
* Initial goal is to trace HDF5, NetCDF, Root, POSIX, etc, any I/O related functions calls
## How to use
* Install Gotcha first, export the path, thanks Elsa Gonsiorowski @llnl 
```
git clone https://github.com/llnl/gotcha
cd gotcha
mkdir build install
cd build
cmake ../ -DCMAKE_INSTALL_PREFIX=../install
make
make install
export GOTCHA=path_gotcha_install
```
* Install HDF5 tracer
```
cd ../..
git clone https://github.com/NERSC/gotcha-io.git
cd gotcha-io
module load cray-hdf5
make 
```

* Load compiler:
```
module load PrgEnv-intel  #for loading Intel compiler #unload other first. Example: module unload PrgEnv-compiler
module swap PrgEnv-intel PrgEnv-cray #for swapping to Cray compiler
module swap PrgEnv-intel PrgEnv-gnu #for swapping to GNU compiler
```

* Test HDF5 apps
```
cd test
source test #issue: if source does not work, try bash
```
## [Gotcha](https://gotcha.readthedocs.io/en/latest/#) is an API that provides function wrapping.

## [Gotcha-Tracer](https://github.com/llnl/GOTCHA-tracer) is a Python tool for creating tracer with Gotcha

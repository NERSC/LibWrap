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
module load cray-hdf5
module load openssl/1.1.0a
export LD_LIBRARY_PATH=/usr/common/software/rabbitmq/0.9.0/lib64:$LD_LIBRARY_PATH 
export JANSSON=/path/to/jannson
make 
```
* Test HDF5 apps
```
cd test
source test
```
* Test HDF5 MPIO apps
``
cd test
source test_p
``
## [Gotcha](https://gotcha.readthedocs.io/en/latest/#) is an API that provides function wrapping.

## [Gotcha-Tracer](https://github.com/llnl/GOTCHA-tracer) is a Python tool for creating tracer with Gotcha

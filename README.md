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

* Generator
To get started go generator/. You can specify the function that you want to wrap in yourmodulename.txt file with following format. 

```
FUNCTIONS
hid_t H5Fcreate(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id)
hid_t H5Fopen( const char *name, unsigned flags, hid_t fapl_id )
herr_t H5Fclose( hid_t file_id )
LOG_WRAP_FUNCTIONS
H5Fcreate_log
H5Fopen_log
H5Fclose_log
LIBRARY
hdf5
LOG_MPI_REDUCE
log_MPI_reduce()
LOG_MPI_FINALIZE
log_MPI_finalize()
MAKE_LOG
make_log()
INCLUDE_HEADERS
hdf5.h
LOG_FILE
log
```
The text file contains keyword followed by user parameters. Only FUNCTIONS keyword is mandatory other are optional. (More description will be added soon)
```
FUNCTIONS - the functions to be wrap
FUNCTIONS_LOG - the wrappers for corresponding functions. Should follow same order
...
INCLUDE_HEADERS - headers from the library function that is being wrapped
LOG_FILE - name of the file where the function definitions reside
```

Once you have specified the parameters do ``python parser yourmodulename``

* Install wrapper
```
module load openssl/1.1.0a
export LD_LIBRARY_PATH=/usr/common/software/rabbitmq/0.9.0/lib64:$LD_LIBRARY_PATH 
export JANSSON=/path/to/c/jannson
make 
```
This step would create yourmodulename.so. You should now link the library to your application when executing for example LD\_PRELOAD=path/to/yourmodule.so:path/to/libgotcha.so ./myapp. See test/ more examples.

## [Gotcha](https://gotcha.readthedocs.io/en/latest/#) is an API that provides function wrapping.

## [Gotcha-Tracer](https://github.com/llnl/GOTCHA-tracer) is a Python tool for creating tracer with Gotcha

## Configuration File
The configuration file contains keyword (in capital) followed by user parameters new line. Only FUNCTIONS keyword is mandatory. Other are optional.
```
FUNCTIONS - The signature of functions to be wrap. One function per line. Also corresponding user wrapper seperated by colon. 
USER_WRAP_FILE - name of the file where all the user defined log functions and data structure resides. You should create both - .c and .h
LOG_MPI_FINALIZE - check below
LOG_ATEXIT - check below
INCLUDE_HEADERS - headers that need to be included of the library functions that is being wrapped
INCLUDES_IN_MAKE and LIBRARIES_IN_MAKE - includes and library that you want to include in MAKEFILE for compilation
STATIC_LIBRARIES - path to the static libraries (.a files) that user want to use statically with their wrapper.
one path per line. Please look at the config file for more example.
OUTPUT_DIR - path to where the files are generated
```

This tool will generate the source files that would include the meta wrappers that has logic needed to do wrapping of the function. The meta wrappers will also call the user wrappers that user can define. The name of the user wrappers are defined after the library functions that need to be wrapped seperated by colon inside FUNCTIONS keyword. The name of the file where the user wrapper functions reside in is given in USER\_WRAP\_FILE. Create a corresponding .h file too. 

This generator will also create Makefile that would compile both user code and generated meta wrapper source code into a single .so or .a file. If the user is using any libraries in the user wrapper code, they should include it in INCLUDES\_IN\_MAKE and LIBRARIES\_IN\_MAKE. 

The user is responsible for formatting and writing the logs. We do not recommend doing the logging inside the user wrappers. We recommend doing the logging during the end of application. To that end, tool accepts two functions that can be definer with LOG\_MPI\_FINALIZE and LOG\_ATEXIT. The corresponding functions would be called at MPI\_finalize and program termination respectively. Check example\_usrwrap/posix/posix\_wrap.c:mpi\_finalize\_cb and example\_usrwrap/posix/posix\_wrap.h:log\_atexit in the for example. 

*Note* -The generator will also generate package config file wrapper-config.pc, where all the libraries(mentioned in STATIC_LIBRARIES in config file, please look at any example config file to get more idea about how to insert path for static libraries) used by the users will be statically linked that will be used by the wrapper. This package config file will be used by the module in case of static linking. Normally in module we set the path to this package config file, so that wrapper can find out which routines to wrap and how to link them with the wrapper statically.


*Note* - except user wrap functions all other functions should have void return type and no parameters.

*Note* - If you specify LOG\_MPI\_FINALIZE then LOG\_ATEXIT will not be called. We assume user will do all logging at MPI\_finalize. However, if application does not use mpi then should ignore MPI\_LOG\_FINALIZE parameter and specify LOG\_ATEXIT.
## Configuration File
The configuration file contains keyword (in capital) followed by user parameters new line. Only FUNCTIONS keyword is mandatory. Other are optional.
```
FUNCTIONS - The signature of functions to be wrap. One function per line.
LOG_WRAP_FUNCTIONS - The wrappers for corresponding functions. Should follow same order as functions
LIBRARY - 
LOG_MPI_FINALIZE - check below
LOG_ATEXIT - check below
INCLUDE_HEADERS - headers that need to be included of the library function that is being wrapped
LOG_FILE - name of the file where all the user defined log functions and data structure resides. This file should be in $LIBWRAPHOME. You should create both - LOG_FILE.c and LOG_FILE.h
```

The tool allows user to extract any infomation/statistics from the functions that are defined in the FUNCTIONS. To do this, the user should create a new log function with same parameters as the functions that needs to be wrap and void return type in new file. The name of the new function should be included in the LOG\_WRAP\_FUNCTIONS and the file name in LOG\_FILE. Inside the new routine the user can define any set of variables/data structures to gather information. 

The user is responsible for formatting and writing the logs. We do not recommend doing the logging inside the wrappers. We recommend doing the logging during the end of application. To that end, tool accepts two functions that can be defined in LOG\_MPI\_FINALIZE and LOG\_ATEXIT. The correspoinding functions would be called at MPI\_finalize and program termination respectively. Check log.c:mpi\_finalize\_cb and log\_atexit in the example\_logfiles for recommended practices. 

*Note* - except log functions all other functions will have void return type and no parameters. 
Check example\_logfile for examples of logfile

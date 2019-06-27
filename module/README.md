## Configuration File
The configuration file contains keyword (in capital) followed by user parameters. Only FUNCTIONS keyword is mandatory. Other are optional.
```
FUNCTIONS - The signature of functions to be wrap. One function per line.
LOG_WRAP_FUNCTIONS - The wrappers for corresponding functions. Should follow same order as functions
LIBRARY - 
LOG_MPI_REDUCE - check below
LOG_MPI_FINALIZE - check below
MAKE_LOG - check below 
INCLUDE_HEADERS - headers that need to be included of the library function that is being wrapped
LOG_FILE - name of the file where all the user defined log functions and data structure resides. This file should be in $LIBWRAPHOME. You should create both - LOG_FILE.c and LOG_FILE.h
```

The tool allows user to extract any infomation/statistics from the functions that are defined in the FUNCTIONS. To do this, the user should create a new log function with same parameters as the functions that needs to be wrap and void return type in new file. The name of the new function should be included in the LOG\_WRAP\_FUNCTIONS and the file name in LOG\_FILE. Inside the new routine the user can define any set of variables/data structures to gather information. 

The tool extracts some default information of the job and logs it using JSON. To include your custom information in the logs, user should define a function (function name goes in MAKE\_LOG) to translate the any gathered statistics in json format. The tool has a json\_t object named root. The user should update this root object in the MAKE\_LOG function with her information. 

#### MPI
To reduce overhead of logging, the tool logs only from one rank. So any information gathered in other ranks should be gathered at the rank 0. User should define a new function that does the reduce using MPI\_COMM\_WORLD communicator at rank 0. The name of this function should go in LOG\_MPI\_FUNCTION. If the user wants to delete the data structures at the end of MPI created during the program, new function should be provided with function name in LOG\_MPI\_FINALIZE.

*Note* - All the functions user defined functions should reside in same LOG\_FILE and except log functions all will have void return type and no parameters. 
Check example\_logfile for examples of logfile

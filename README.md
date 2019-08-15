## Reflector Wrapper library
* Wrapper library that can be used to wrap any function from any library

## Quick Overview

* Reflector helps in intercepting both dynamic and static functions used in user applications. Reflector operates with the following steps:
	- A configuration file (see at example\_ready\_hdf5\_wrapper\hdf5.config) needs to be created which contains all the parameters provided by a wrapper author
	- The Reflector uses the configuration file to generate meta wrappers ( see example\_ready\_hdf5\_wrapper\mywrapper\wrapper.c)
	- Wrapper author needs to define user wrappers ( see example\_ready\_hdf5\_wrapper\mywrapper\h5\_wrap.c). The meta wrappers (i.e., wrapper functions in example\_ready\_hdf5\_wrapper\wrapper.c) will call the user wrappers.
	- Wrapper author also needs to provide the logging mechanism (see example\_ready\_hdf5\_wrapper\mywrapper\h5\_wrap.c: mpi\_finalize\_cb and log\_atexit).
	- The Makefile will generate then the wrapper library (N.B. in case of static linking a special script is needed to be executed before running Makefile, see example\_ready\_hdf5\_wrapper\mywrapper\static-object-generator.sh)
	- A ready to use basic hdf5 wrapper (H5Fcreate, H5Fopen, H5Fclose) is provided at example\_ready\_hdf5\_wrapper\mywrapper. (please run Makefile to generate wrapper)

## Dependencies

If you want to use scripts in example\_logfile/ then you need

* JANSSON 2.12 or higher
* openssl-1.1.0a or higher (default in cori)
* [rabbitmq-c](https://github.com/alanxz/rabbitmq-c) 0.9.0 or higher 


** Important Note: By default the environment is static at Cori at this moment. However,  you can also set dynamic or static mode through CRAYPE_LINK_TYPE variable

** Shared/Dynamic build-
```
export CRAYPE_LINK_TYPE=dynamic
```

** Static build-
You need to write a module to load your package config file. The package config contains the links for all the statically linked routines and path to the necessary libraries, where those routines aredefined. Please look at example config file in /module to see how to provide paths for third party dependencies. An example of module (i.e.,module-dynamic-v0.0.2) is given here. Besides, please look at /module to get more idea about how to configure your package_config_file in a module file.
```
export CRAYPE_LINK_TYPE=static

```

** Load Dependencies-
```
module load reflector-hdf5 ### Helps to load all the dependencies: rabbitmq,jansson

OR

export LD_LIBRARY_PATH=/global/common/cori_cle6/software/jansson/lib:/global/common/cori_cle6/software/rabbitmq/0.9.0/lib64:$LD_LIBRARY_PATH 
```  

## How to generate custom wrapper with Reflector (Step by Step)
* There are 2 components of this tool
	- Meta Wrappers
	- User Wrappers

Meta wrappers contains the logic to intercept the function calls from the library. We use GOTCHA and GNU-Linker (with package.config) to enable function interception. Tool will generate this wrapper code from user parameters provied in config file (see at module/hdf5.config). Check Readme in module/ directory. User wrappers is written by users and contains the logic of collecting statistics that the user is interested in extracting. The meta wrappers will call user wrappers. Some example of such user wrappers are in the example_usrwrap. 

* To get started go module/. You should create a configuration file (e.g., module/hdf5.config) with functions that needs to be wrap and other necessary parameters. More details in module/

* Once a configuration file (see at /module/hdf5.config) has been created run ``python parse.py yourmodulename``. This will create meta wrapper files and Makefile in output specified in the config file. We will assume output is in mywrapper/ 

* Write your user wrappers as the names defined in the config file (example_wrap\rand is simplest example). For this tutorial, you can copy posix_wrap from example_usrwrap/posix to mywrapper/. posix_wrap in the example is submitting logs to MODS. The files for sending data to MODS resides in example_outlog/MODS/. Copy all files to mywrapper/. 

* Install wrapper inside mywrapper/
```
bash static-object-generator.sh  ### This step is specifically for static mode that helps to extract object files from the static libraries provided by user.
make ### Will create the wrapper library
```

Your wrapper libraries (yourmodulename.so and yourmodule.a) should be created at mywrapper/. 

** Test the wrapper-
* You should now link the library to your application when executing for example `LD_PRELOAD=path/to/yourmodule.so ./myapp`. See test/ for more examples. For testing you can do in test dir `bash test_posix.sh`. This should have submitted data in [MODS](https://kb.nersc.gov). 

* To locate your data select Default space in MODS -> Go to Discover. Set RoutingKey (this is drop down just below Add a filter on left) to ou-das-\*. Search for your username or "wraplib-posix". Dont forget to select the correct time range on top right.

**Test with module- In module file you can set LD_PRELOAD to link your shared wrapper(i.e., wrapper.so) with the application  and can also set Package_Config_Path to load the static wrapper (i.e., wrapper.a). In Cori you need to have your module in Cori's /global/common/cori_cle6/software/modulefiles/ in order to use the module.


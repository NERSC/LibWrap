## Unified Wrapper library
* wrapper library that can be used to wrap any function from any library

## Dependencies
* [Gotcha](https://gotcha.readthedocs.io/en/latest/#). Gotcha is an API that provides function wrapping.

If you want to use scripts in example\_logfile/ then you need

* JANSSON 2.12 or higher
* openssl-1.1.0a or higher 
* [rabbitmq-c](https://github.com/alanxz/rabbitmq-c) 0.9.0 or higher 


** Important Note: You need to set dynamic or static mode through CRAYPE_LINK_TYPE variable
```
export CRAYPE_LINK_TYPE = dynamic
OR
export CRAYPE_LINK_TYPE = static 
```

** Shared/Dynamic mode-
```
export CRAYPE_LINK_TYPE=dynamic
module load gnu-wrap #will load the dependencies openssl,rabbitmq and jansson
```

** Static mode-
You need to write a module to load your package config file. The package config contains the links for all the statically linked routines and path to the necessary libraries, where those routines aredefined. Please look at example config file in /module to see how to provide paths for third party dependencies. An example of module (i.e.,module-libwrap-01) is given here. Besides, please look under the module section to get more idea about how to configure your package config file in a module file.
```
export CRAYPE_LINK_TYPE=static
module load your-module-for-loading-your-pkg-config
```
** Quick testing the wrapper without module-
Please look at test that contains test scripts for testing your wrapper both in static and shared mode. In Cori you need to have your module in Cori's /usr/commom/software/modulefiles in order to use the module.

*Note*: Use the rabbitmq and openssl version as specified here. As of June 27 2019, the default versiof rabbitmq and openssl on cori are incompatible. You will **not** get any errors while using incompatible versions and might end up spending hours figuring out the issue. 
  

## How to use
* To get started go module/. You should create a configuration file with functions that needs to be wrap and other necessary parameters. More details in module/

* Once a configuration file has been created run ``python parser yourmodulename``. This will create necessary files and Makefile in $libwrap home. 

* Install wrapper
```
export GOTCHA=path/to/gotcha/install ### We can move this step to module for both static and shared mode
bash static-object-generator.sh  ### This step is specifically for static mode that helps to extract object files from the static libraries provided by user.
make 
```
** Please look at example_outlog and example_usrwrap to see sample user log file and sample techniqueto produce output to MODS. The user log files and the MODS file need to be in $libwrap home.

* This step would create yourmodulename.so and yourmodule.a. You should now link the library to your application when executing for example LD\_PRELOAD=path/to/yourmodule.so:path/to/libgotcha.so ./myapp. See test/ for more examples.

**Note- In module file (i.e., module gnu-wrap) you can set LD_LIBRARY_PATH to link the wrapper with the application for shared mode and can also set Package_Config_Path for static mode.


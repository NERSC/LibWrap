## Wrapper library
* wrapper library that can be used to wrap functions from any library

## Dependencies
* [Gotcha](https://gotcha.readthedocs.io/en/latest/#). Gotcha is an API that provides function wrapping.

If you want to follow tutorial below to submit data to MODS then you will need

* JANSSON 2.12 or higher
* openssl-1.1.0a or higher 
* [rabbitmq-c](https://github.com/alanxz/rabbitmq-c) 0.9.0 or higher 

On cori you can load these using

```
module load openssl/1.1.0a
export LD_LIBRARY_PATH=/usr/common/software/rabbitmq/0.9.0/lib64:$LD_LIBRARY_PATH 
```

*Note*: Use the rabbitmq and openssl version as specified here. As of June 27 2019, the default versiof rabbitmq and openssl on cori are incompatible.
*Note*: Depending on your compiled application(cray/intel/gnu), you should use the same compiled version of GOTCHA
 
## How to use
* There are 2 components of this tool
	- Meta Wrappers
	- User Wrappers
Meta wrappers contains the logic to intercept the function calls from the library. We use GOTCHA and package.config to do this. Tool will generate this code from user parameters. Check Readme in module/ directory. User wrappers is written by users and contains the logic of collecting statistics that the user is interested in extracting. The meta wrappers will call user wrappers. Some example of such user wrappers are in the example\_usrwrap. 

* To get started go to module/. You should create a configuration file with functions that needs to be wrap and other necessary parameters. More details about the parameters and examples are in module/

* Once a configuration file has been created run ``python2 parse.py yourmodulename``. This will create meta wrapper files and Makefile in output specified in the config file. We will assume output is mywrapper/ 

* Write your user wrappers as the names defined in the config file (example\_wrap\rand is simplest example). For this tutorial, you can copy posix\_wrap from example\_usrwrap/posix to mywrapper/. posix\_wrap in the example is submitting logs to MODS. The files for sending data to MODS resides in example\_outlog/MODS/. Copy all files to mywrapper/.

* Install wrapper inside mywrapper/
```
export GOTCHA=path_gotcha_install
make 
```
This step would create yourmodulename.so and yourmodule.a. 

* You should now link the library to your application when executing for example `LD\_PRELOAD=path/to/yourmodule.so:path/to/libgotcha.so ./myapp`. See test/ for more examples. For testing you can do in test dir `bash test\_posix.sh`. This should have submitted data in [MODS](https://kb.nersc.gov). 

* To locate your data select Default space in MODS -> Go to Discover. Set RoutingKey (this is drop down just below Add a filter on left) to ou-das-\*. Search for your username or "wraplib-posix". Dont forget to select the correct time range on top right.


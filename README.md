# Static lib wrapper
'''
Prepare the pkg-config file first. Pkg-config file must contain the following information:
'''
Pkg-config path (i.e., prefix)
Wrapper library path (i.e., wrapper_libdir, see example below)
Linking information (i.e., Libs)
 
 
'''
Set PKG_CONFIG_PATH with the PATH of your pkg-config file
'''
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/global/homes/a/aalmamun/dyninst-10.1.0/examples/wrap/pkg-check

'''
Prepare the module. Module file must contain information about at-least two things:
'''
PKG_CONFIG
Wrapper library 

'''
Set MODULEPATH
'''
module use your_module_path/module_name

This should automatically set the MODULEPATH to your module directory.

Load module: module load module_name

Note: Cori needs the module to be in the default directory. Once the module is in the default directory, we do not need to set MODULEPATH anymore. Just load it and use it.


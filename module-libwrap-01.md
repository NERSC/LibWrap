#%Module
set     name        swrap
set     version     0.0.1
set     root        /usr/common/software/$name/$version

set     fullname    swrap
set     externalurl https://github.com/NERSC/gotcha-io
set     nerscurl    https://www.nersc.gov
set     maincategory    applications
set     subcategory "programming"
set     description "wrapper using gnu linker"

proc ModulesHelp { } {
        global description nerscurl externalurl
        puts stderr "Description - $description"
        puts stderr "NERSC Docs  - $nerscurl"
        puts stderr "Other Docs  - $externalurl"
}

set PREFIX /global/homes/a/aalmamun/dyninst-10.1.0/examples/wrap/pkg-check


if { [ info exists ::env(CRAYPE_LINK_TYPE) ] } {
    if {$env(CRAYPE_LINK_TYPE) == "dynamic"} {
    
       # remove-path PKG_CONFIG_PATH $PREFIX
       # remove-path PE_PKGCONFIG_LIBS wrapper
       # prepend-path LD_PRELOAD $PREFIX/mywrapper/libgotchah5.so:$PREFIX/mywrapper/libgotcha.so
        module load openssl/1.1.0a
        prepend-path LD_LIBRARY_PATH /usr/common/software/rabbitmq/0.9.0/lib64
        prepend-path LD_LIBRARY_PATH /usr/common/software/gotcha/install/lib64
        prepend-path LD_LIBRARY_PATH /usr/common/software/jansson/lib
        prepend-path GOTCHA /usr/common/software/gotcha/1.0
  } elseif {$env(CRAYPE_LINK_TYPE) == "static"} {
      
       # prepend-path PKG_CONFIG_PATH $PREFIX
       # prepend-path PE_PKGCONFIG_LIBS wrapper
       # for static mode: gotcha, openssl, rabbitmq, jansson will be statically linked by Makefile 
  }

} else {
    
#        prepend-path PKG_CONFIG_PATH $PREFIX
#        prepend-path PE_PKGCONFIG_LIBS wrapper
}


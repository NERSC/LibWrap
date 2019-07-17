mkdir ALL_OBJECTS
cd ALL_OBJECTS
ar -xv /LibWrap/dockers/static/libssl.a
ar -xv /LibWrap/dockers/static/libcrypto.a
ar -xv /LibWrap/dockers/static/librabbitmq.a
ar -xv /LibWrap/dockers/static/libjansson.a
ar -xv /LibWrap/dockers/static/libhdf5_parallel.a
ar -xv /LibWrap/dockers/static/libhdf5_intel.a
ar -xv /LibWrap/dockers/static/libgotcha.a


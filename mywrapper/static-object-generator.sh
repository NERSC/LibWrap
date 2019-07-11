mkdir ALL_OBJECTS
cd ALL_OBJECTS
ar -xv $(HOME)/LibWrap/dockers/static/libssl.a
ar -xv $(HOME)/LibWrap/dockers/static/libcrypto.a
ar -xv $(HOME)/LibWrap/dockers/static/librabbitmq.a
ar -xv $(HOME)/LibWrap/dockers/static/libjansson.a
ar -xv $(HOME)/LibWrap/dockers/static/libhdf5_parallel.a
ar -xv $(HOME)/LibWrap/dockers/static/libhdf5_intel.a
ar -xv $(HOME)/LibWrap/dockers/static/libgotcha.a


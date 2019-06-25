#include <hdf5.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "gotcha/gotcha.h"
#include <pthread.h>
#include <mpi.h>
#include <hdf5.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "log_init.h"
#include "log.h"

#define NFUNCS 3


hid_t __wrap_H5Fopen(const char *name, unsigned flags, hid_t fapl_id){
	return __real_H5Fopen(name, flags, fapl_id));
}

static gotcha_wrappee_handle_t H5Fopen_handle;
static hid_t (*H5Fopen) (const char *name, unsigned flags, hid_t fapl_id);
static hid_t H5Fopen (const char *name, unsigned flags, hid_t fapl_id){
	H5Fopen = gotcha_get_wrappee(H5Fopen_handle);
	return H5Fopen ? (H5Fopen(name, flags, fapl_id)) : 0;
}

herr_t __wrap_H5Fclose_wrapper(hid_t file_id){
	return __real_H5Fclose_wrapper(file_id));
}

static gotcha_wrappee_handle_t H5Fclose_wrapper_handle;
static herr_t (*H5Fclose_wrapper) (hid_t file_id);
static herr_t H5Fclose_wrapper (hid_t file_id){
	H5Fclose_wrapper = gotcha_get_wrappee(H5Fclose_wrapper_handle);
	return H5Fclose_wrapper ? (H5Fclose_wrapper(file_id)) : 0;
}

hid_t __wrap_H5Fcreate_wrapper(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id){
	return __real_H5Fcreate_wrapper(name, flags, fcpl_id, fapl_id));
}

static gotcha_wrappee_handle_t H5Fcreate_wrapper_handle;
static hid_t (*H5Fcreate_wrapper) (const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id);
static hid_t H5Fcreate_wrapper (const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id){
	H5Fcreate_wrapper = gotcha_get_wrappee(H5Fcreate_wrapper_handle);
	return H5Fcreate_wrapper ? (H5Fcreate_wrapper(name, flags, fcpl_id, fapl_id)) : 0;
}


struct gotcha_binding_t wrap_hdf5 [] = {
	{ "H5Fopen", H5Fopen, &H5Fopen },
	{ "H5Fclose_wrapper", H5Fclose_wrapper, &H5Fclose_wrapper },
	{ "H5Fcreate_wrapper", H5Fcreate_wrapper, &H5Fcreate_wrapper },
};

void init_gotcha_hdf5 () {
	gotcha_wrap(wrap_hdf5, NFUNCS, "wrapper");
}

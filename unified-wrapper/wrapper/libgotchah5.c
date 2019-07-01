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

#define NFUNCS 1

ssize_t __real_H5Fcreate(const char *name,unsigned flags,hid_t fcpl_id,hid_t fapl_id);
pthread_mutex_t log_mutex;

static gotcha_wrappee_handle_t H5Fcreate_handle;
typedef hid_t (*H5Fcreate_fptr)(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id);

int __wrap_H5Fcreate(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id){

	pthread_mutex_lock(&log_mutex);
	H5Fcreatee_log(name, flags, fcpl_id, fapl_id);
	pthread_mutex_unlock(&log_mutex);
	printf("\nstatic wrapped\n");
	hid_t result=__real_H5Fcreate(name, flags, fcpl_id, fapl_id);
	return result;
	//return __real_H5Fcreate(name, flags, fcpl_id, fapl_id);
}

static hid_t H5Fcreate_wrapper(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id){

	H5Fcreate_fptr H5Fcreate_wrappee = (H5Fcreate_fptr)gotcha_get_wrappee(H5Fcreate_handle);

	hid_t result=H5Fcreate_wrappee(name, flags, fcpl_id, fapl_id);

	pthread_mutex_lock(&log_mutex);
	H5Fcreatee_log(name, flags, fcpl_id, fapl_id);
	pthread_mutex_unlock(&log_mutex);
	printf("\ndynamic wrapped\n");
	 return result;
}

void H5Fcreatee_log(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id){
	printf("This is log");
	return;}


static gotcha_binding_t wrap_hdf5 [] = {
	{ "H5Fcreate", H5Fcreate_wrapper, &H5Fcreate_handle }
};

void init_gotcha_hdf5(){
	gotcha_wrap(wrap_hdf5, NFUNCS, "wrapper");
}
__attribute__((constructor)) void construct_me(){
	init_gotcha_hdf5();
}

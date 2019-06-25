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

#define NFUNCS 1


static gotcha_wrappee_handle_t H5Fopen_handle;
static hid_t (*H5Fopen)(const char *name, unsigned flags, hid_t fapl_id);

int __wrap_H5Fopen(const char *name, unsigned flags, hid_t fapl_id){

	pthread_mutex_lock(&log_mutex);
	H5Fopen_log(name, flags, fapl_id);
	pthread_mutex_unlock(&log_mutex);

	return __real_H5Fopen(name, flags, fapl_id);
}

static hid_t H5Fopen_wrapper(const char *name, unsigned flags, hid_t fapl_id){

	H5Fopen H5Fopen_wrappee= gotcha_get_wrappee(H5Fopen_handle);

	hid_t result=H5Fopen_wrappee(name, flags, fapl_id);

	pthread_mutex_lock(&log_mutex);
	H5Fopen_log(name, flags, fapl_id);
	pthread_mutex_unlock(&log_mutex);

	 return result;
}

void H5Fopen_log(const char *name, unsigned flags, hid_t fapl_id){
	printf("This is log");
}


struct gotcha_binding_t wrap_hdf5 [] = {
	{ "H5Fopen", H5Fopen, &H5Fopen },
};

void init_gotcha_hdf5(){
	gotcha_wrap(wrap_hdf5, NFUNCS, "wrapper");
}
__attribute__((constructor)) void construct_me(){
	init_gotcha_hdf5();
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "gotcha/gotcha.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* User included headers*/
#include <hdf5.h>

#include "log_init.h"
#include "h5_wrap.h"

#define NFUNCS 3


pthread_mutex_t log_mutex;

static gotcha_wrappee_handle_t H5Fcreate_handle;
typedef hid_t (*H5Fcreate_fptr)(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id);

static gotcha_wrappee_handle_t H5Fopen_handle;
typedef hid_t (*H5Fopen_fptr)( const char *name, unsigned flags, hid_t fapl_id );

static gotcha_wrappee_handle_t H5Fclose_handle;
typedef herr_t (*H5Fclose_fptr)( hid_t file_id );



int __wrap_H5Fcreate(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	H5Fcreate_mywrap(name, flags, fcpl_id, fapl_id);
	pthread_mutex_unlock(&log_mutex);

	return __real_H5Fcreate(name, flags, fcpl_id, fapl_id);
}


static hid_t H5Fcreate_wrapper(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	H5Fcreate_mywrap(name, flags, fcpl_id, fapl_id);
	pthread_mutex_unlock(&log_mutex);

	H5Fcreate_fptr H5Fcreate_wrappee = (H5Fcreate_fptr)gotcha_get_wrappee(H5Fcreate_handle);

	hid_t result=H5Fcreate_wrappee(name, flags, fcpl_id, fapl_id);

	return result;
}


int __wrap_H5Fopen( const char *name, unsigned flags, hid_t fapl_id ){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	H5Fopen_mywrap(name, flags, fapl_id);
	pthread_mutex_unlock(&log_mutex);

	return __real_H5Fopen(name, flags, fapl_id);
}


static hid_t H5Fopen_wrapper( const char *name, unsigned flags, hid_t fapl_id ){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	H5Fopen_mywrap(name, flags, fapl_id);
	pthread_mutex_unlock(&log_mutex);

	H5Fopen_fptr H5Fopen_wrappee = (H5Fopen_fptr)gotcha_get_wrappee(H5Fopen_handle);

	hid_t result=H5Fopen_wrappee(name, flags, fapl_id);

	return result;
}


int __wrap_H5Fclose( hid_t file_id ){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	H5Fclose_mywrap(file_id);
	pthread_mutex_unlock(&log_mutex);

	return __real_H5Fclose(file_id);
}


static herr_t H5Fclose_wrapper( hid_t file_id ){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	H5Fclose_mywrap(file_id);
	pthread_mutex_unlock(&log_mutex);

	H5Fclose_fptr H5Fclose_wrappee = (H5Fclose_fptr)gotcha_get_wrappee(H5Fclose_handle);

	herr_t result=H5Fclose_wrappee(file_id);

	return result;
}



static gotcha_binding_t wrap_hdf5 [] = {
	{ "H5Fcreate", H5Fcreate_wrapper, &H5Fcreate_handle },
	{ "H5Fopen", H5Fopen_wrapper, &H5Fopen_handle },
	{ "H5Fclose", H5Fclose_wrapper, &H5Fclose_handle }
};


void init_gotcha_hdf5(){
	gotcha_wrap(wrap_hdf5, NFUNCS, "wrapper");
}


__attribute__((constructor)) void construct_me(){
	init_gotcha_hdf5();
}

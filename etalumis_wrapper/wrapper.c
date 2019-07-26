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
//#include <fcntl.h>
//#include <gdbm.h>

#include "log_init.h"
#include "posix_wrap.h"

#define NFUNCS 3


pthread_mutex_t log_mutex;

static gotcha_wrappee_handle_t open_handle;
typedef int (*open_fptr)(const char *pathname, int flags, mode_t mode);

int __wrap_open(const char *pathname, int flags, mode_t mode);

static gotcha_wrappee_handle_t write_handle;
typedef ssize_t (*write_fptr)(int fd, const void *buf, size_t count);

ssize_t __wrap_write(int fd, const void *buf, size_t count);

static gotcha_wrappee_handle_t read_handle;
typedef ssize_t (*read_fptr)(int fd, void *buf, size_t count);

ssize_t __wrap_read(int fd, void *buf, size_t count);
/*
static gotcha_wrappee_handle_t gdbm_fetch_handle;
typedef datum (*gdbm_fetch_fptr)(GDBM_FILE dbf, datum key);

datum __wrap_gdbm_fetch(GDBM_FILE dbf, datum key);
*/


int __wrap_open(const char *pathname, int flags, mode_t mode){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	open_mywrap(pathname, flags, mode);
	pthread_mutex_unlock(&log_mutex);

	return __real_open(pathname, flags, mode);
}


static int open_wrapper(const char *pathname, int flags, mode_t mode){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	open_mywrap(pathname, flags, mode);
	pthread_mutex_unlock(&log_mutex);

	open_fptr open_wrappee = (open_fptr)gotcha_get_wrappee(open_handle);

	return open_wrappee(pathname, flags, mode);
}


ssize_t __wrap_write(int fd, const void *buf, size_t count){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	write_mywrap(fd, buf, count);
	pthread_mutex_unlock(&log_mutex);

	return __real_write(fd, buf, count);
}


static ssize_t write_wrapper(int fd, const void *buf, size_t count){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	write_mywrap(fd, buf, count);
	pthread_mutex_unlock(&log_mutex);

	write_fptr write_wrappee = (write_fptr)gotcha_get_wrappee(write_handle);

	return write_wrappee(fd, buf, count);
}


ssize_t __wrap_read(int fd, void *buf, size_t count){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	read_mywrap(fd, buf, count);
	pthread_mutex_unlock(&log_mutex);

	return __real_read(fd, buf, count);
}


static ssize_t read_wrapper(int fd, void *buf, size_t count){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	read_mywrap(fd, buf, count);
	pthread_mutex_unlock(&log_mutex);

	read_fptr read_wrappee = (read_fptr)gotcha_get_wrappee(read_handle);

	return read_wrappee(fd, buf, count);
}

/*
datum __wrap_gdbm_fetch(GDBM_FILE dbf, datum key){

	pthread_mutex_lock(&log_mutex);
	_log_init();
	gdbm_fetch_mywrap(dbf, key);
	pthread_mutex_unlock(&log_mutex);

	return __real_gdbm_fetch(dbf, key);
}
*/
/*
static datum gdbm_fetch_wrapper(GDBM_FILE dbf, datum key){
	printf("this is in wrapper.c func: gdbm fetch\n");
	pthread_mutex_lock(&log_mutex);
	_log_init();
	gdbm_fetch_mywrap(dbf, key);
	pthread_mutex_unlock(&log_mutex);

	gdbm_fetch_fptr gdbm_fetch_wrappee = (gdbm_fetch_fptr)gotcha_get_wrappee(gdbm_fetch_handle);

	return gdbm_fetch_wrappee(dbf, key);
}

*/

static gotcha_binding_t wrap_posix_rdwt [] = {
	{ "open", open_wrapper, &open_handle },
	{ "write", write_wrapper, &write_handle },
	{ "read", read_wrapper, &read_handle }
//	{ "gdbm_fetch", gdbm_fetch_wrapper, &gdbm_fetch_handle }
};


void init_gotcha_posix_rdwt(){
	gotcha_wrap(wrap_posix_rdwt, NFUNCS, "wrapper");
}


__attribute__((constructor)) void construct_me(){
	init_gotcha_posix_rdwt();
}

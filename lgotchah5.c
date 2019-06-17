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

#include "log.h"

typedef hid_t (*H5Fcreate_fptr)(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id);
typedef hid_t (*H5Fopen_fptr)(const char *name, unsigned flags, hid_t fapl_id);
typedef hid_t (*H5Fclose_fptr)(hid_t file_id);
typedef herr_t (*H5Dread_fptr)(void *dset, hid_t mem_type_id, hid_t mem_space_id,
	                        hid_t file_space_id, hid_t plist_id, void *buf);
typedef herr_t (*H5Dwrite_fptr)(void *dset, hid_t mem_type_id, hid_t mem_space_id,
				hid_t file_space_id, hid_t plist_id, void *buf);

static gotcha_wrappee_handle_t H5Fcreate_handle;
static gotcha_wrappee_handle_t H5Fopen_handle;
static gotcha_wrappee_handle_t H5Fclose_handle;
static gotcha_wrappee_handle_t H5Dread_handle;
static gotcha_wrappee_handle_t H5Dwrite_handle;
/*
int read_count = 0;
int create_count = 0;
int close_count = 0;
int dread_count = 0;
int dwrite_count = 0;
*/
double tot_log_overhead = 0; 
pthread_mutex_t log_mutex;

static void printoverhead()
{
  fprintf(stderr, "Total overhead gotcha time %f sec\n", tot_log_overhead);
  return ;
}


static hid_t H5Fcreate_wrapper(const char * name, unsigned flags, hid_t fcpl_id, 
								hid_t fapl_id) 
{
  clock_t t_start = clock();
  H5Fcreate_fptr H5Fcreate_wrappee = (H5Fcreate_fptr) gotcha_get_wrappee(H5Fcreate_handle);
  
  clock_t t1 = clock();
  hid_t result = H5Fcreate_wrappee(name, flags, fcpl_id, fapl_id);
  clock_t t2 = clock();
  
  pthread_mutex_lock(&log_mutex);
  H5Fcreate_log(name, flags, fcpl_id, fapl_id);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  return result;
}


static hid_t H5Fopen_wrapper(const char *name, unsigned flags, hid_t fapl_id) 
{
  clock_t t_start = clock();
  H5Fopen_fptr H5Fopen_wrappee = (H5Fopen_fptr) gotcha_get_wrappee(H5Fopen_handle);
  
  clock_t t1 = clock();
  hid_t result = H5Fopen_wrappee(name, flags, fapl_id);
  clock_t t2 = clock();

  pthread_mutex_lock(&log_mutex);
  H5Fopen_log(name, flags, fapl_id);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC; 
  return result;
}


static hid_t H5Fclose_wrapper(hid_t file_id) 
{
  clock_t t_start = clock();
  H5Fclose_fptr H5Fclose_wrappee = (H5Fclose_fptr) gotcha_get_wrappee(H5Fclose_handle);
  
  clock_t t1 = clock();
  hid_t result = H5Fclose_wrappee(file_id);
  clock_t t2 = clock();

  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  //printoverhead();
  return result;
}


static herr_t H5Dread_wrapper(void *dset, hid_t mem_type_id, hid_t mem_space_id,
                      hid_t file_space_id, hid_t plist_id, void *buf)
{ 
  clock_t t_start = clock();
  H5Dread_fptr H5Dread_wrappee = (H5Dread_fptr) gotcha_get_wrappee(H5Dread_handle);
  
  clock_t t1 = clock();
  herr_t ret_val = H5Dread_wrappee(dset, mem_type_id, mem_space_id, file_space_id, 
							plist_id, buf );
  clock_t t2 = clock();
  
  pthread_mutex_lock(&log_mutex);
  H5Dread_log(dset, mem_type_id, mem_space_id, file_space_id, plist_id, buf);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  return ret_val;
}


static herr_t H5Dwrite_wrapper(void *dset, hid_t mem_type_id, hid_t mem_space_id,
			hid_t file_space_id, hid_t plist_id, void *buf)
{
  clock_t t_start=clock();
  H5Dwrite_fptr H5Dwrite_wrappee = (H5Dwrite_fptr) gotcha_get_wrappee(H5Dwrite_handle);
  
  clock_t t1 = clock();
  herr_t ret_val = H5Dwrite_wrappee(dset, mem_type_id, mem_space_id, file_space_id,
							plist_id, buf);
  clock_t t2 = clock();
 
  pthread_mutex_lock(&log_mutex);
  H5Dwrite_log(dset, mem_type_id, mem_space_id, file_space_id, plist_id, buf);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  return ret_val;
}


static gotcha_binding_t h5_file_func_bindings[]={
  {"H5Fcreate", H5Fcreate_wrapper, &H5Fcreate_handle},
  {"H5Fopen", H5Fopen_wrapper, &H5Fopen_handle},
  {"H5Fclose", H5Fclose_wrapper, &H5Fclose_handle},
  {"H5Dread", H5Dread_wrapper, &H5Dread_handle},
  {"H5Dwrite", H5Dwrite_wrapper, &H5Dwrite_handle}
};


void init_gotcha_h5_file_func(){
  gotcha_wrap(h5_file_func_bindings, 5, "h5gotcha");
}


__attribute__((constructor)) void construct_me(){
  init_gotcha_h5_file_func();
}



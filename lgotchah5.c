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
typedef herr_t (*H5Fclose_fptr)(hid_t file_id);

typedef hid_t (*H5Acreate2_fptr)(hid_t loc_id, const char *attr_name, hid_t type_id, 
					hid_t space_id, hid_t acpl_id, hid_t aapl_id); 
typedef hid_t (*H5Aopen_fptr)(hid_t obj_id, const char *attr_name, hid_t aapl_id); 
typedef herr_t (*H5Awrite_fptr)(hid_t attr_id, hid_t mem_type_id, const void *buf);
typedef herr_t (*H5Aread_fptr)(hid_t attr_id, hid_t mem_type_id, void *buf);
typedef herr_t (*H5Aclose_fptr)(hid_t attr_id);

typedef hid_t (*H5Dcreate2_fptr)(hid_t loc_id, const char *name, hid_t dtype_id, 
			hid_t space_id, hid_t lcpl_id, hid_t dcpl_id, hid_t dapl_id);
typedef hid_t (*H5Dopen2_fptr)(hid_t loc_id, const char *name, hid_t dapl_id);
typedef herr_t (*H5Dwrite_fptr)(void *dset, hid_t mem_type_id, hid_t mem_space_id,
				hid_t file_space_id, hid_t plist_id, void *buf);
typedef herr_t (*H5Dread_fptr)(void *dset, hid_t mem_type_id, hid_t mem_space_id,
	                        hid_t file_space_id, hid_t plist_id, void *buf);
typedef herr_t (*H5Dclose_fptr)(hid_t dataset_id);

typedef hid_t (*H5Gcreate2_fptr)(hid_t loc_id, const char *name, hid_t lcpl_id, 
						hid_t gcpl_id, hid_t gapl_id);
typedef hid_t (*H5Gopen2_fptr)(hid_t loc_id, const char * name, hid_t gapl_id);
typedef herr_t (*H5Gclose_fptr)(hid_t group_id);


static gotcha_wrappee_handle_t H5Fcreate_handle;
static gotcha_wrappee_handle_t H5Fopen_handle;
static gotcha_wrappee_handle_t H5Fclose_handle;

static gotcha_wrappee_handle_t H5Acreate2_handle;
static gotcha_wrappee_handle_t H5Aopen_handle;
static gotcha_wrappee_handle_t H5Awrite_handle;
static gotcha_wrappee_handle_t H5Aread_handle;
static gotcha_wrappee_handle_t H5Aclose_handle;

static gotcha_wrappee_handle_t H5Dcreate2_handle;
static gotcha_wrappee_handle_t H5Dopen2_handle;
static gotcha_wrappee_handle_t H5Dwrite_handle;
static gotcha_wrappee_handle_t H5Dread_handle;
static gotcha_wrappee_handle_t H5Dclose_handle;

static gotcha_wrappee_handle_t H5Gcreate2_handle;
static gotcha_wrappee_handle_t H5Gopen2_handle;
static gotcha_wrappee_handle_t H5Gclose_handle;


double tot_log_overhead = 0; 
pthread_mutex_t log_mutex;

//TODO: send this to mods
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


static herr_t H5Fclose_wrapper(hid_t file_id) 
{
  clock_t t_start = clock();
  H5Fclose_fptr H5Fclose_wrappee = (H5Fclose_fptr) gotcha_get_wrappee(H5Fclose_handle);
  
  clock_t t1 = clock();
  herr_t result = H5Fclose_wrappee(file_id);
  clock_t t2 = clock();

  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  //printoverhead();
  return result;
}


static hid_t H5Acreate2_wrapper(hid_t loc_id, const char *attr_name, hid_t type_id, 
					hid_t space_id, hid_t acpl_id, hid_t aapl_id) 
{
  clock_t t_start = clock();
  H5Acreate2_fptr H5Acreate2_wrappee = (H5Acreate2_fptr) gotcha_get_wrappee(H5Acreate2_handle);
  
  clock_t t1 = clock();
  hid_t result = H5Acreate2_wrappee(loc_id, attr_name, type_id, space_id, acpl_id, aapl_id);
  clock_t t2 = clock();
  
  pthread_mutex_lock(&log_mutex);
  H5Acreate2_log(loc_id, attr_name, type_id, space_id, acpl_id, aapl_id);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  return result;
}


static hid_t H5Aopen_wrapper(hid_t obj_id, const char *attr_name, hid_t aapl_id) 
{
  clock_t t_start = clock();
  H5Aopen_fptr H5Aopen_wrappee = (H5Aopen_fptr) gotcha_get_wrappee(H5Aopen_handle);
  
  clock_t t1 = clock();
  hid_t result = H5Aopen_wrappee(obj_id, attr_name, aapl_id);
  clock_t t2 = clock();

  pthread_mutex_lock(&log_mutex);
  H5Aopen_log(obj_id, attr_name, aapl_id);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC; 
  return result;
}


static herr_t H5Awrite_wrapper(hid_t attr_id, hid_t mem_type_id, const void *buf)
{
  clock_t t_start=clock();
  H5Awrite_fptr H5Awrite_wrappee = (H5Awrite_fptr) gotcha_get_wrappee(H5Awrite_handle);
  
  clock_t t1 = clock();
  herr_t ret_val = H5Awrite_wrappee(attr_id, mem_type_id, buf);
  clock_t t2 = clock();
 
  pthread_mutex_lock(&log_mutex);
  H5Awrite_log(attr_id, mem_type_id, buf);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  return ret_val;
}


static herr_t H5Aread_wrapper(hid_t attr_id, hid_t mem_type_id, void *buf)
{ 
  clock_t t_start = clock();
  H5Aread_fptr H5Aread_wrappee = (H5Aread_fptr) gotcha_get_wrappee(H5Aread_handle);
  
  clock_t t1 = clock();
  herr_t ret_val = H5Aread_wrappee(attr_id, mem_type_id, buf);
  clock_t t2 = clock();
  
  pthread_mutex_lock(&log_mutex);
  H5Aread_log(attr_id, mem_type_id, buf);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  return ret_val;
}


static herr_t H5Aclose_wrapper(hid_t attr_id) 
{
  clock_t t_start = clock();
  H5Aclose_fptr H5Aclose_wrappee = (H5Aclose_fptr) gotcha_get_wrappee(H5Aclose_handle);
  
  clock_t t1 = clock();
  herr_t result = H5Aclose_wrappee(attr_id);
  clock_t t2 = clock();

  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  //printoverhead();
  return result;
}


static hid_t H5Dcreate2_wrapper(hid_t loc_id, const char *name, hid_t dtype_id, 
			hid_t space_id, hid_t lcpl_id, hid_t dcpl_id, hid_t dapl_id)
{
  clock_t t_start = clock();
  H5Dcreate2_fptr H5Dcreate2_wrappee = (H5Dcreate2_fptr) gotcha_get_wrappee(H5Dcreate2_handle);
  
  clock_t t1 = clock();
  hid_t ret_val = H5Dcreate2_wrappee(loc_id, name, dtype_id, space_id, 
					   lcpl_id, dcpl_id, dapl_id);
  clock_t t2 = clock();
  
  pthread_mutex_lock(&log_mutex);
  H5Dcreate2_log(loc_id, name, dtype_id, space_id, lcpl_id, dcpl_id, dapl_id);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  return ret_val;
}


static hid_t H5Dopen2_wrapper(hid_t loc_id, const char *name, hid_t dapl_id)
{
  clock_t t_start = clock();
  H5Dopen2_fptr H5Dopen2_wrappee = (H5Dopen2_fptr) gotcha_get_wrappee(H5Dopen2_handle);
  
  clock_t t1 = clock();
  hid_t ret_val = H5Dopen2_wrappee(loc_id, name, dapl_id);
  clock_t t2 = clock();
  
  pthread_mutex_lock(&log_mutex);
  H5Dopen2_log(loc_id, name, dapl_id);
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


static herr_t H5Dclose_wrapper(hid_t dataset_id)
{
  clock_t t_start = clock();
  H5Dclose_fptr H5Dclose_wrappee = (H5Dclose_fptr) gotcha_get_wrappee(H5Dclose_handle);
  
  clock_t t1 = clock();
  herr_t ret_val = H5Dclose_wrappee(dataset_id);
  clock_t t2 = clock();
  
  pthread_mutex_lock(&log_mutex);
  H5Dclose_log(dataset_id);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  return ret_val;
}


static hid_t H5Gcreate2_wrapper(hid_t loc_id, const char *name, hid_t lcpl_id, 
						hid_t gcpl_id, hid_t gapl_id)
{
  clock_t t_start = clock();
  H5Gcreate2_fptr H5Gcreate2_wrappee = (H5Gcreate2_fptr) gotcha_get_wrappee(H5Gcreate2_handle);
  
  clock_t t1 = clock();
  hid_t ret_val = H5Gcreate2_wrappee(loc_id, name, lcpl_id, gcpl_id, gapl_id);
  clock_t t2 = clock();
  
  pthread_mutex_lock(&log_mutex);
  H5Gcreate2_log(loc_id, name, lcpl_id, gcpl_id, gapl_id);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  return ret_val;
}


static hid_t H5Gopen2_wrapper(hid_t loc_id, const char * name, hid_t gapl_id)
{
  clock_t t_start = clock();
  H5Gopen2_fptr H5Gopen2_wrappee = (H5Gopen2_fptr) gotcha_get_wrappee(H5Gopen2_handle);
  
  clock_t t1 = clock();
  hid_t ret_val = H5Gopen2_wrappee(loc_id, name, gapl_id);
  clock_t t2 = clock();
  
  pthread_mutex_lock(&log_mutex);
  H5Gopen2_log(loc_id, name, gapl_id);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  return ret_val;
}


static herr_t H5Gclose_wrapper(hid_t group_id)
{
  clock_t t_start = clock();
  H5Gclose_fptr H5Gclose_wrappee = (H5Gclose_fptr) gotcha_get_wrappee(H5Gclose_handle);
  
  clock_t t1 = clock();
  herr_t ret_val = H5Gclose_wrappee(group_id);
  clock_t t2 = clock();
  
  pthread_mutex_lock(&log_mutex);
  H5Gclose_log(group_id);
  pthread_mutex_unlock(&log_mutex);
  
  clock_t t_tot = clock()-t_start;
  tot_log_overhead += ((double)(t_tot-(t2-t1)))/CLOCKS_PER_SEC;
  return ret_val;
}


static gotcha_binding_t h5_file_func_bindings[]={
  {"H5Fcreate", H5Fcreate_wrapper, &H5Fcreate_handle},
  {"H5Fopen", H5Fopen_wrapper, &H5Fopen_handle},
  {"H5Fclose", H5Fclose_wrapper, &H5Fclose_handle},
  
  {"H5Acreate2", H5Acreate2_wrapper, &H5Acreate2_handle},
  {"H5Aopen", H5Aopen_wrapper, &H5Aopen_handle},
  {"H5Awrite", H5Awrite_wrapper, &H5Awrite_handle},
  {"H5Aread", H5Aread_wrapper, &H5Aread_handle},
  {"H5Aclose", H5Aclose_wrapper, &H5Aclose_handle},
  
  {"H5Dcreate2", H5Dcreate2_wrapper, &H5Dcreate2_handle},
  {"H5Dopen2", H5Dopen2_wrapper, &H5Dopen2_handle},
  {"H5Dwrite", H5Dwrite_wrapper, &H5Dwrite_handle},
  {"H5Dread", H5Dread_wrapper, &H5Dread_handle},
  {"H5Dclose", H5Dclose_wrapper, &H5Dclose_handle},
  
  {"H5Gcreate2", H5Gcreate2_wrapper, &H5Gcreate2_handle},
  {"H5Gopen2", H5Gopen2_wrapper, &H5Gopen2_handle},
  {"H5Gclose", H5Gclose_wrapper, &H5Gclose_handle}
};


void init_gotcha_h5_file_func(){
  gotcha_wrap(h5_file_func_bindings, 16, "h5gotcha");
}


__attribute__((constructor)) void construct_me(){
  init_gotcha_h5_file_func();
}



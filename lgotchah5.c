#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "gotcha/gotcha.h"
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
double tot_time = 0; 


static void printoverhead()
{
  fprintf(stderr, "Total overhead gotcha time %f sec\n", tot_time);
  return ;
}


static hid_t H5Fcreate_wrapper(const char * name, unsigned flags, hid_t fcpl_id, 
								hid_t fapl_id) 
{
  clock_t t = clock();
  H5Fcreate_fptr H5Fcreate_wrappee = (H5Fcreate_fptr) gotcha_get_wrappee(H5Fcreate_handle);
  t = clock()-t;
  tot_time += ((double)t)/CLOCKS_PER_SEC;
  hid_t result = H5Fcreate_wrappee(name, flags, fcpl_id, fapl_id);
  t = clock();
  //create_count++;
  H5Fcreate_log(name, flags, fcpl_id, fapl_id);
  t = clock()-t;
  tot_time += ((double)t)/CLOCKS_PER_SEC;
  return result;
}


static hid_t H5Fopen_wrapper(const char *name, unsigned flags, hid_t fapl_id) 
{
  clock_t t = clock();
  H5Fopen_fptr H5Fopen_wrappee = (H5Fopen_fptr) gotcha_get_wrappee(H5Fopen_handle);
  t = clock()-t;
  tot_time += ((double)t)/CLOCKS_PER_SEC;
  hid_t result = H5Fopen_wrappee(name, flags, fapl_id);
  t = clock();
  //open_count++;
  H5Fopen_log(name, flags, fapl_id);
  t = clock()-t;
  tot_time += ((double)t)/CLOCKS_PER_SEC; 
  return result;
}


static hid_t H5Fclose_wrapper(hid_t file_id) 
{
  clock_t t = clock();
  H5Fclose_fptr H5Fclose_wrappee = (H5Fclose_fptr) gotcha_get_wrappee(H5Fclose_handle);
  t = clock()-t;
  tot_time +=((double)t)/CLOCKS_PER_SEC;
  hid_t result = H5Fclose_wrappee(file_id);
  //fprintf(stderr, "H5Fclose(%0llx) = %0llx\n",
  //        file_id,result);
  t = clock();
  //close_count++;
  //wrt_log(result, "close", 0);
  t = clock()-t;
  tot_time += ((double)t)/CLOCKS_PER_SEC;
  return result;
}


static herr_t H5Dread_wrapper(void *dset, hid_t mem_type_id, hid_t mem_space_id,
                      hid_t file_space_id, hid_t plist_id, void *buf)
{ 
  H5Dread_fptr H5Dread_wrappee = (H5Dread_fptr) gotcha_get_wrappee(H5Dread_handle);
  herr_t ret_val = H5Dread_wrappee(dset, mem_type_id, mem_space_id, file_space_id, 
							plist_id, buf );
  //dread_count++;
  //do_log(NULL, NULL, ret_val, "DRead");
  return ret_val;
}


static herr_t H5Dwrite_wrapper(void *dset, hid_t mem_type_id, hid_t mem_space_id,
			hid_t file_space_id, hid_t plist_id, void *buf)
{
  H5Dwrite_fptr H5Dwrite_wrappee = (H5Dwrite_fptr) gotcha_get_wrappee(H5Dwrite_handle);
  herr_t ret_val = H5Dwrite_wrappee(dset, mem_type_id, mem_space_id, file_space_id,
							plist_id, buf);
  //dwrite_count++;
  //do_log(NULL, NULL, ret_val, "DWrite");
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



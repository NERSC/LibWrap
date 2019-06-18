#include <stdio.h>
#include <stdlib.h>
#include "gotcha/gotcha.h"
//#include <hdf5.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//typedef hid_t (*H5Fopen_fptr)(const char * name, unsigned flags, hid_t fapl_id);
typedef int  (*open_fptr)(const char* Path, int flags);
typedef int  (*close_fptr)(int fd);

static gotcha_wrappee_handle_t H5Fopen_handle;
static gotcha_wrappee_handle_t open_handle;
static gotcha_wrappee_handle_t close_handle;



/*static hid_t H5Fcreate_wrapper(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id) {
  H5Fcreate_fptr H5Fcreate_wrappee = (H5Fcreate_fptr) gotcha_get_wrappee(H5Fcreate_handle);
  hid_t result = H5Fcreate_wrappee(name, flags, fcpl_id, fapl_id);
  fprintf(stderr, "H5Fcreate(%s, %u, %0llx, %0llx) = %0llx\n",
          name, flags, fcpl_id, fapl_id, result);
  return result;
}*/

/*static hid_t H5Fopen_wrapper(const char *name, unsigned flags, hid_t fapl_id) {
  H5Fopen_fptr H5Fopen_wrappee = (H5Fopen_fptr) gotcha_get_wrappee(H5Fopen_handle);
  hid_t result = H5Fopen_wrappee(name, flags, fapl_id);
  fprintf(stderr, "H5Fopen(%s, %u, %0llx) = %0llx\n",
          name, flags, fapl_id, result);
  printf("HDF5  open wrapper from shared lib\n");
  return result;
}*/

static int open_wrapper(const char* name, int flags) {
  open_fptr open_wrappee = (open_fptr) gotcha_get_wrappee(open_handle);
  int result = open_wrappee(name, flags);
  //fprintf(stderr, "H5Fopen(%s, %u, %0llx) = %0llx\n", name, flags, fapl_id, result);
  printf("\n open wrapper from shared lib\n");
  return result;
}

static int close_wrapper(int id) {
  close_fptr close_wrappee = (close_fptr) gotcha_get_wrappee(close_handle);
  int result = close_wrappee(id);
  //fprintf(stderr, "H5Fclose(%0llx) = %0llx\n",      file_id,result);
  printf("\n close wrapper from shared lib\n");
  return result;
}


static gotcha_binding_t func_bindings[]={
   //{"H5Fopen", H5Fopen_wrapper, &H5Fopen_handle},
   {"open", open_wrapper, &open_handle},
   {"close", close_wrapper, &close_handle}
};
void init_gotcha_file_func(){
 gotcha_wrap(func_bindings, 2, "SAgotcha");
}


__attribute__((constructor)) void construct_me(){
    init_gotcha_file_func();
}



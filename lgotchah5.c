#include <stdio.h>
#include <stdlib.h>
#include "gotcha/gotcha.h"
#include <hdf5.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
typedef hid_t (*H5Fcreate_fptr)(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id);
typedef hid_t (*H5Fopen_fptr)(const char *name, unsigned flags, hid_t fapl_id);
typedef hid_t (*H5Fclose_fptr)(hid_t file_id);

static gotcha_wrappee_handle_t H5Fcreate_handle;
static gotcha_wrappee_handle_t H5Fopen_handle;
static gotcha_wrappee_handle_t H5Fclose_handle;



static hid_t H5Fcreate_wrapper(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id) {
  H5Fcreate_fptr H5Fcreate_wrappee = (H5Fcreate_fptr) gotcha_get_wrappee(H5Fcreate_handle);
  hid_t result = H5Fcreate_wrappee(name, flags, fcpl_id, fapl_id);
  fprintf(stderr, "H5Fcreate(%s, %u, %0llx, %0llx) = %0llx\n",
          name, flags, fcpl_id, fapl_id, result);
  return result;
}

static hid_t H5Fopen_wrapper(const char *name, unsigned flags, hid_t fapl_id) {
  H5Fopen_fptr H5Fopen_wrappee = (H5Fopen_fptr) gotcha_get_wrappee(H5Fopen_handle);
  hid_t result = H5Fopen_wrappee(name, flags, fapl_id);
  fprintf(stderr, "H5Fopen(%s, %u, %0llx) = %0llx\n",
          name, flags, fapl_id, result);
  return result;
}

static hid_t H5Fclose_wrapper(hid_t file_id) {
  H5Fclose_fptr H5Fclose_wrappee = (H5Fclose_fptr) gotcha_get_wrappee(H5Fclose_handle);
  hid_t result = H5Fclose_wrappee(file_id);
  fprintf(stderr, "H5Fclose(%0llx) = %0llx\n",
          file_id,result);
  return result;
}


static gotcha_binding_t h5_file_func_bindings[]={
   {"H5Fcreate", H5Fcreate_wrapper, &H5Fcreate_handle},
   {"H5Fopen", H5Fopen_wrapper, &H5Fopen_handle},
   {"H5Fclose", H5Fclose_wrapper, &H5Fclose_handle}
};
void init_gotcha_h5_file_func(){
 gotcha_wrap(h5_file_func_bindings, 3, "h5gotcha");
}


__attribute__((constructor)) void construct_me(){
    init_gotcha_h5_file_func();
}



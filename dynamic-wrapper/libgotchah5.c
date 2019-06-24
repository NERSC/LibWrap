#include <stdio.h>
#include <stdlib.h>
#include "gotcha/gotcha.h"
//#include <hdf5.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <dlfcn.h>

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

static int open_common(int is_static, const char* name, int flags)
{
    typedef int  (*open_fptr)(const char* Path, int flags);
    open_fptr open_wrappee;

    // Common "logging" command
    printf("open wrapper, is_static = %d\n", is_static);
    // End common "logging" command

    if(is_static)
        open_wrappee = __real_open(name,flags);
    else
        open_wrappee = (open_fptr) gotcha_get_wrappee(open_handle);

    return open_wrappee(name, flags);
}

static int open_wrapper(const char* name, int flags)
{
    printf("Open shared wrapper\n");
    return open_common(0, name, flags);
}

int __wrap_open(char *name, int flags)
{
    printf("Open static  wrapper");
    return open_common(1, name, flags);
}


/*static int open_wrapper(const char* name, int flags) {
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

int __wrap_open(char *arg,int flag){
        printf("wrap POSIX open\n");
       // real_open = dlsym(RTLD_NEXT, "open");
        //real_open(arg,flag);
        return __real_open(arg,flag);
        //return 0;
}

int __wrap_close(char *arg,int flag){
        printf("wrap POSIX close\n");
        return 0;
}*/

static gotcha_binding_t func_bindings[]={
   //{"H5Fopen", H5Fopen_wrapper, &H5Fopen_handle},
   {"open", open_wrapper, &open_handle}
//   {"close", close_wrapper, &close_handle}
};
void init_gotcha_file_func(){
 gotcha_wrap(func_bindings, 1, "SAgotcha");
}


__attribute__((constructor)) void construct_me(){
    init_gotcha_file_func();
}



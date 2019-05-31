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

typedef hid_t (*H5Fcreate_fptr)(const char * name, unsigned flags, hid_t fcpl_id, hid_t fapl_id);
typedef hid_t (*H5Fopen_fptr)(const char *name, unsigned flags, hid_t fapl_id);
typedef hid_t (*H5Fclose_fptr)(hid_t file_id);

static gotcha_wrappee_handle_t H5Fcreate_handle;
static gotcha_wrappee_handle_t H5Fopen_handle;
static gotcha_wrappee_handle_t H5Fclose_handle;


static hbool_t is_mpi(hid_t fapl_id)
{
  hbool_t have_mpi = false;
  hid_t driver_id;
  // If its the default fapl then we know its serial
  if (fapl_id == H5P_DEFAULT){
    have_mpi = false;
  }
  else if((driver_id = H5Pget_driver(fapl_id)) > 0 && driver_id == H5FD_MPIO){
    have_mpi = true;
  }
  return have_mpi;
}


static void wrt_log(const char *name, unsigned flags, hid_t fapl_id, hid_t result)
{
  fprintf(stderr, "H5Fopen(%s, %u, %0llx) = %0llx at ts=%lu uid=%d\n",
          name, flags, fapl_id, result, (unsigned long)time(NULL), getuid());
  return ;
}


static void do_log(const char *name, unsigned flags, hid_t fapl_id, hid_t result)
{
  // Check if its MPI
  if (is_mpi(fapl_id)){
    fprintf(stderr, "Using MPI\n");
    MPI_Comm mpi_comm = MPI_COMM_NULL;
    MPI_Info mpi_info = MPI_COMM_NULL;
    H5Pget_fapl_mpio(fapl_id, &mpi_comm, &mpi_info);
    
    // Get world size and rank
    int world_size, world_rank;
    MPI_Comm_size(mpi_comm, &world_size);
    MPI_Comm_rank(mpi_comm, &world_rank);
    // fprintf(stderr, "Total communicators %d\n", world_size);
    // fprintf(stderr, "My rank %d\n", world_rank); 
    // Write from only one rank per communicator
    if (world_rank == 0)
    	wrt_log(name, flags, fapl_id, result);
  }
  else{
    wrt_log(name, flags, fapl_id, result);
  }
  return ;
}


static hid_t H5Fcreate_wrapper(const char * name, unsigned flags, hid_t fcpl_id, 
								hid_t fapl_id) 
{
  H5Fcreate_fptr H5Fcreate_wrappee = (H5Fcreate_fptr) gotcha_get_wrappee(H5Fcreate_handle);
  hid_t result = H5Fcreate_wrappee(name, flags, fcpl_id, fapl_id);
  do_log(name, flags, fapl_id, result);
  return result;
}


static hid_t H5Fopen_wrapper(const char *name, unsigned flags, hid_t fapl_id) 
{
  H5Fopen_fptr H5Fopen_wrappee = (H5Fopen_fptr) gotcha_get_wrappee(H5Fopen_handle);
  hid_t result = H5Fopen_wrappee(name, flags, fapl_id);
  do_log(name, flags, fapl_id, result);
  return result;
}


static hid_t H5Fclose_wrapper(hid_t file_id) 
{
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



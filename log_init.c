#include "log_init.h"

int flag_isparallel=0;
static int flag_atexit = 0;

int key_val; /* To add attribute to MPI comm for cb */

void serial_atexit()
{
  if (!flag_isparallel){
    extrct_log_info();
    make_log(); 
    add_job_info();  
    send_to_mods();
  }
  return ;
}


static int mpi_log_cb(MPI_Comm comm, int keyval, void *attr_val, int *flag)
{
  /* Reduce operations */
  log_MPI_reduce();
  int world_rank;
  MPI_Comm_rank(comm, &world_rank);
  if (world_rank == 0)
  {
    extrct_log_info();
    make_log(); 
    add_job_info();  
    send_to_mods();
  }
  /* Clean up */
  log_MPI_finalize();
  reset_job_log();  
  return MPI_SUCCESS;	
}


void mpi_atexit()
{ 
  int mpi_code;  
  /* add an attribute on MPI_COMM_WORLD to call send_to_mods()
     when it is destroyed, i.e. on MPI_Finalize */
  if(MPI_SUCCESS != (mpi_code = MPI_Comm_create_keyval(MPI_COMM_NULL_COPY_FN, 
  					(MPI_Comm_delete_attr_function *)mpi_log_cb, 
									&key_val, NULL))){
    fprintf(stderr, "MPI_Comm_create_keyval failed");
    return ;
  }
  if(MPI_SUCCESS != (mpi_code = MPI_Comm_set_attr(MPI_COMM_WORLD, key_val, NULL))){
    fprintf(stderr, "MPI_Comm_set_attr failed");
    return ;
  }
  if(MPI_SUCCESS != (mpi_code = MPI_Comm_free_keyval(&key_val))){
    fprintf(stderr, "MPI_Comm_free_keyval failed");
    return ;
  }
  return ;
}


void log_init()
{ 
  /* If its using MPI call mpi_atexit */
  if (!flag_isparallel)
  {
    int mpi_initialized;
    int mpi_finalized;
 
    MPI_Initialized(&mpi_initialized);
    MPI_Finalized(&mpi_finalized);
    if (mpi_initialized && !mpi_finalized){ 
      mpi_atexit();
      flag_isparallel=1;
      flag_atexit=1;
    }
  }
  if (!flag_atexit)
  {
    atexit(serial_atexit);
    flag_atexit=1;
  }
}

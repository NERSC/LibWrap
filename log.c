#include "log.h"

struct log_info job_log; 
struct api_counts loc_parallel_api_counts, serial_api_counts, glo_parallel_api_counts;
static int tot_serial_api_count = 0;
static int tot_parallel_api_count = 0;

int world_size = -1;
int world_rank = -1;

MPI_Comm mpi_comm = MPI_COMM_NULL;
MPI_Info mpi_info = MPI_COMM_NULL;

hbool_t is_mpi(hid_t fapl_id)
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


void extrct_log_info(const int ismpi)
{
  unsigned long long ts = (unsigned long)time(NULL);
  time_t rtime;
  time(&rtime);
  struct tm * info = localtime(&rtime);
   
  job_log.ismpi = ismpi;
  job_log.uid = getuid(); 
  job_log.first_hdf5api_time = asctime(info); 
  job_log.host = getenv("NERSC_HOST");
  job_log.user = getenv("USER");
  job_log.hostname = getenv("hostname");
  //job_log.slurm_job_id = ()
  job_log.slurm_job_num_nodes = getenv("SLURM_JOB_NUM_NODES");
  job_log.slurm_job_account = getenv("SLURM_JOB_ACCOUNT");
  
  //This is part to get the nodetype
  job_log.nodetype="None";  
  if (strcmp(job_log.host, "cori")==0){
    FILE *fptr = fopen("/proc/cpuinfo", "r");
    if (fptr == NULL)
      fprintf(stderr, "failed to open /proc/cpuinfo\n");
    char* line=NULL;
    size_t len;
    ssize_t read;
    while ((read=getline(&line, &len, fptr))!=-1){
      char tempstr[10];
      strncpy(tempstr, line, 10);
      if (strcmp(tempstr, "model name")==0){
        if (strstr(line, "Phi")!=NULL)
          job_log.nodetype = "KNL";
        else {
          job_log.nodetype = "Haswell";
        }
        break; 
      }
    }
    fclose(fptr);
  }
  //TODO: get this working
  /*  
  if (strcmp(job_log.hostname, "nid")==0)
    job_log.is_compute = 1;
  else
    job_log.is_compute = 0; 
  */
  return ;
}


void reset_api_counts(struct api_counts my_api_counts)
{
  my_api_counts.open_count = 0;
  my_api_counts.create_count = 0; 
  my_api_counts.close_count = 0;
  my_api_counts.dread_count = 0;
  my_api_counts.dwrite_count = 0;
  return ;
}


static int mpi_send_mods_cb(MPI_Comm comm, int keyval, void *attr_val, int *flag)
{
  fprintf(stderr, "Local sum for process %d - %d\n",
       world_rank, loc_parallel_api_counts.open_count);
  // Reduce all of the local sums into the global sum
  MPI_Reduce(&loc_parallel_api_counts.open_count, &glo_parallel_api_counts.open_count, 
					1, MPI_INT, MPI_SUM, 0, mpi_comm);
  
  if(world_rank==0){
    send_to_mods();
    fprintf(stderr, "Global sum for process %d - %d\n",
       world_rank, glo_parallel_api_counts.open_count);
  }
  
  // Destroy all MPI information
  world_rank = -1;
  world_size = -1;
  /*
  mpi_comm = MPI_COMM_NULL;
  mpi_info = MPI_COMM_NULL;
  */
  // Reset all counts
  tot_parallel_api_count = 0;
  reset_api_counts(loc_parallel_api_counts);
  reset_api_counts(glo_parallel_api_counts); 
  // Reset log information
  job_log.host = NULL;
  job_log.user = NULL;
  job_log.hostname = NULL;
  job_log.slurm_job_id = NULL;
  job_log.slurm_job_num_nodes = NULL;
  job_log.slurm_job_account = NULL;
  job_log.nodetype = NULL;
  job_log.is_compute = -1;
  job_log.first_hdf5api_time = NULL;
  job_log.ismpi = -1;
  job_log.uid = -1;
  
  return MPI_SUCCESS;
}


void mpi_atexit()
{ 
  int mpi_initialized;
  int mpi_finalized;
  int mpi_code;
  
  MPI_Initialized(&mpi_initialized);
  MPI_Finalized(&mpi_finalized);
   
  /* add an attribute on MPI_COMM_SELF to call send_to_mods()
     when it is destroyed, i.e. on MPI_Finalize */
  if (mpi_initialized && !mpi_finalized) {
      int key_val;
      if(MPI_SUCCESS != (mpi_code = MPI_Comm_create_keyval(MPI_COMM_NULL_COPY_FN, 
  					(MPI_Comm_delete_attr_function *)mpi_send_mods_cb, 
									&key_val, NULL))){
  	//HMPI_GOTO_ERROR(FAIL, "MPI_Comm_create_keyval failed", mpi_code)
        fprintf(stderr, "MPI_Comm_create_keyval failed");
        return ;
      }
      if(MPI_SUCCESS != (mpi_code = MPI_Comm_set_attr(MPI_COMM_SELF, key_val, NULL))){
  	//HMPI_GOTO_ERROR(FAIL, "MPI_Comm_set_attr failed", mpi_code)
        fprintf(stderr, "MPI_Comm_set_attr failed");
        return ;
      }
      if(MPI_SUCCESS != (mpi_code = MPI_Comm_free_keyval(&key_val))){
        //HMPI_GOTO_ERROR(FAIL, "MPI_Comm_free_keyval failed", mpi_code)
        fprintf(stderr, "MPI_Comm_free_keyval failed");
        return ;
      }
  }
  return ;
}


void mpi_extrct_log_info(hid_t fapl_id)
{
  //MPI_Comm_size(mpi_comm, &world_size);
  H5Pget_fapl_mpio(fapl_id, &mpi_comm, &mpi_info);
  //printf("%s:%u - <message>\n",__func__, __LINE__);
  MPI_Comm_rank(mpi_comm, &world_rank);
  
  // Write from only one rank per communicator
  if (world_rank == 0)
    extrct_log_info(1);
  return ;
}


void mpi_log(hid_t fapl_id)
{
  //printf("%s:%u - <message>\n",__func__, __LINE__);
  if (tot_parallel_api_count == 0){
    mpi_extrct_log_info(fapl_id);
    //printf("%s:%u - <message>\n",__func__, __LINE__);
    mpi_atexit();
    //printf("%s:%u - <message>\n",__func__, __LINE__);
  }
  tot_parallel_api_count++;
  return ;
}


void serial_log()
{
  if (tot_serial_api_count == 0){
    extrct_log_info(0);
    printf("%s:%u - <message>\n",__func__, __LINE__);
    atexit(send_to_mods);
  }
  tot_serial_api_count++;
  return ;
}

//TODO: H5get_fapl_mpio does not work
void H5Fcreate_log(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id)
{
  //printf("%s:%u - <message>\n",__func__, __LINE__);
  if (is_mpi(fapl_id)){
    //mpi_log(fapl_id);
    loc_parallel_api_counts.create_count++;
  }
  else{
    serial_log(); 
    serial_api_counts.create_count++;
  }
  return ;
}


void H5Fopen_log(const char *name, unsigned flags, hid_t fapl_id)
{
  //printf("%s:%u - <message>\n",__func__, __LINE__);
  if (is_mpi(fapl_id)){
    mpi_log(fapl_id);
    loc_parallel_api_counts.open_count++;
  }
  else{
    serial_log();
    serial_api_counts.open_count++;
  } 
  return ;
}

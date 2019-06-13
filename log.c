#include "log.h"

struct log_info job_log; 

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


void wrt_log(hid_t result, const char *file_op, const int ismpi)
{
  static int count = 0;
  // computes only job information only once
  if (!count){
     
    job_log.ismpi = ismpi;
    job_log.uid = getuid(); 
    unsigned long long ts = (unsigned long)time(NULL);
    time_t rtime;
    time(&rtime);
    struct tm * info = localtime(&rtime);
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
  //atexit(send_to_mods);
  }
  //fprintf(stderr,"%d\n",count);
  /*  
  if (strcmp(job_log.hostname, "nid")==0)
    job_log.is_compute = 1;
  else
    job_log.is_compute = 0; 
  */
  count ++;
  return ;
}


void do_log(const char *name, hid_t fapl_id, hid_t result, const char *file_op)
{
   
  // Check if its MPI
  if (is_mpi(fapl_id)){
    //fprintf(stderr, "Using MPI\n");
    MPI_Comm mpi_comm = MPI_COMM_NULL;
    MPI_Info mpi_info = MPI_COMM_NULL;
    H5Pget_fapl_mpio(fapl_id, &mpi_comm, &mpi_info);
    
    // Get world size and rank
    int world_size, world_rank;
    //MPI_Comm_size(mpi_comm, &world_size);
    MPI_Comm_rank(mpi_comm, &world_rank);
    // fprintf(stderr, "Total communicators %d\n", world_size);
    // fprintf(stderr, "My rank %d\n", world_rank); 
    // Write from only one rank per communicator
    if (world_rank == 0)
    	wrt_log(result, file_op, 1);
  }
  else{
    wrt_log(result, file_op, 0);
  }
 
  return ;
}

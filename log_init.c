#include "log.h"


int flag_atexit = 0;
int flag_isparallel = 0;

/* To add attribute to MPI comm for cb */
int key_val;


void extrct_log_info()
{
  unsigned long long ts = (unsigned long)time(NULL);
  time_t rtime;
  time(&rtime);
  struct tm * info = localtime(&rtime);
  //Dont need this 
  //job_log.ismpi = ismpi;
  job_log.uid = getuid(); 
  job_log.first_hdf5api_time = asctime(info); 
  job_log.host = getenv("NERSC_HOST");
  job_log.user = getenv("USER");
  //TODO: This seems to take time. check.
  //char hostnamebuffer[1024];
  //gethostname(hostnamebuffer, sizeof(hostnamebuffer));
  //puts(hostnamebuffer);
  //strcpy(job_log.hostname, hostnamebuffer);
  
  //job_log.slurm_job_id = ()
  job_log.slurm_job_num_nodes = getenv("SLURM_JOB_NUM_NODES");
  job_log.slurm_job_account = getenv("SLURM_JOB_ACCOUNT");
  
  // This is part to get the nodetype
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


void serial_atexit()
{
  if (!flag_isparallel){
   extrct_log_info(); 
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
    send_to_mods();
  }
  /* Clean up */
  log_MPI_finalize();  
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
  int mpi_initialized;
  int mpi_finalized;
 
  MPI_Initialized(&mpi_initialized);
  MPI_Finalized(&mpi_finalized);
  
  /* If its using MPI call mpi_atexit */
  if (!flag_isparallel && (mpi_initialized && !mpi_finalized) )
  {
    mpi_atexit();
    flag_isparallel=1;
    flag_atexit=1;
  }
  if (!flag_atexit)
  {
    atexit(serial_atexit);
    flag_atexit=1;
  }
}

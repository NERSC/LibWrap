#include "posix_wrap.h"

int count_read = 0;
int count_write = 0;

json_t *json_root; 
struct log_info job_log;

/* This creates a json object that can be passed to send_to_mods 
Add your data here */
void crt_json_log()
{
  /* Job information gathered from extrct_job_info */
  json_root = json_object();
  json_object_set_new(json_root, "category", json_string("MODS") );
  json_object_set_new(json_root, "name", json_string("gotchaio-tracer") );
  json_object_set_new(json_root, "uid", json_integer(job_log.uid) );
  json_object_set_new(json_root, "nersc host", json_string(job_log.host));
  json_object_set_new(json_root, "hostname", json_string(job_log.hostname));
  json_object_set_new(json_root, "user", json_string(job_log.user));
  json_object_set_new(json_root, "slurm number of nodes", json_string(job_log.slurm_job_num_nodes));
  json_object_set_new(json_root, "slurm job account", json_string(job_log.slurm_job_account));
  json_object_set_new(json_root, "nodetype", json_string(job_log.nodetype));
  json_object_set_new(json_root, "is_compute_node", json_integer(job_log.is_compute));
  json_object_set_new(json_root, "slurm job id", json_string(job_log.slurm_job_id));
 
  json_object_set_new(json_root, "Number of Reads calls", json_integer(count_read)); 
  json_object_set_new(json_root, "Number of Write calls", json_integer(count_write)); 

  return ;
}


void log_MPI_reduce()
{
  /* Reduce all of the local sums into the global sum */
 
  return ;
}

/* Should have void return return and no parameters*/
void log_atexit()
{
  /* This extracts job information */
  extrct_job_info(&job_log);
  crt_json_log();
  /* Sends data to MODS */
  send_to_mods(json_root);
  return ;	
}


void mpi_finalize_cb()
{
  MPI_Comm comm = MPI_COMM_WORLD;
  /* Reduce operations */
  log_MPI_reduce();
  int world_rank;
  MPI_Comm_rank(comm, &world_rank);
  if (world_rank == 0)
  {
    extrct_job_info(&job_log);
    crt_json_log();
    send_to_mods(json_root);
  }
  
  return ;	
}


/***************************/
/* User wrappers start here */
/***************************/

/* Simple wrapper that counts the number of times read/write is called */

/* Return type is always void 
The parameter is same as the original function that you want to wrap */ 
void write_mywrap(int fd, const void *buf, size_t count)
{
  count_write++;   
  return ;
}


void read_mywrap(int fd, void *buf, size_t count)
{
  count_read++;
  return ;
}

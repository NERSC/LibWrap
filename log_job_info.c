#include "log_job_info.h"

json_t *root;
struct log_info job_log; 


void reset_job_log()
{
  //Reset log information
  job_log.host = NULL;
  job_log.user = NULL;
  job_log.hostname = NULL;
  job_log.slurm_job_id = NULL;
  job_log.slurm_job_num_nodes = NULL;
  job_log.slurm_job_account = NULL;
  job_log.nodetype = NULL;
  job_log.is_compute = -1;
  job_log.uid = -1;
  return ;
}

 
void extrct_log_info()
{
  //unsigned long long ts = (unsigned long)time(NULL);
  //time_t rtime;
  //time(&rtime);
  //struct tm * info = localtime(&rtime);
  job_log.uid = getuid(); 
  //job_log.first_hdf5api_time = asctime(info); 
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


void add_job_info()
{
  root = json_object();
  json_object_set_new(root, "category", json_string("MODS") );
  json_object_set_new(root, "name", json_string("gotchaio-tracer") );
  json_object_set_new(root, "uid", json_integer(job_log.uid) );
  json_object_set_new(root, "nersc host", json_string(job_log.host));
  json_object_set_new(root, "hostname", json_string(job_log.hostname));
  json_object_set_new(root, "user", json_string(job_log.user));
  json_object_set_new(root, "slurm number of nodes", json_string(job_log.slurm_job_num_nodes));
  json_object_set_new(root, "slurm job account", json_string(job_log.slurm_job_account));
  json_object_set_new(root, "nodetype", json_string(job_log.nodetype));
  
  return ;
}

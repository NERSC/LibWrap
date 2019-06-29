def generate_job_log_info(fl_nm):
	c_str ="""#include "log_job_info.h"

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

//TODO: add job id
void extrct_job_info(struct log_info *job_log)
{
  //unsigned long long ts = (unsigned long)time(NULL);
  //time_t rtime;
  //time(&rtime);
  //struct tm * info = localtime(&rtime);
  job_log->uid = getuid(); 
  //job_log.first_hdf5api_time = asctime(info); 
  job_log->host = getenv("NERSC_HOST");
  job_log->user = getenv("USER");
  //TODO: This seems to take time. check.
  //char hostnamebuffer[1024];
  //gethostname(hostnamebuffer, sizeof(hostnamebuffer));
  //puts(hostnamebuffer);
  //strcpy(job_log.hostname, hostnamebuffer);
  
  //job_log.slurm_job_id = ()
  job_log->slurm_job_num_nodes = getenv("SLURM_JOB_NUM_NODES");
  job_log->slurm_job_account = getenv("SLURM_JOB_ACCOUNT");
  
  // This is part to get the nodetype
  job_log->nodetype="None";  
  if (strcmp(job_log->host, "cori")==0){
    FILE *fptr = fopen("/proc/cpuinfo", "r");
    if (fptr == NULL)
      fprintf(stderr, "failed to open /proc/cpuinfo\\n");
    char* line=NULL;
    size_t len;
    ssize_t read;
    while ((read=getline(&line, &len, fptr))!=-1){
      char tempstr[10];
      strncpy(tempstr, line, 10);
      if (strcmp(tempstr, "model name")==0){
        if (strstr(line, "Phi")!=NULL)
          job_log->nodetype = "KNL";
        else {
          job_log->nodetype = "Haswell";
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

"""

	with open("%s.c" % fl_nm, "w") as fl_out:
		fl_out.write(c_str)

	h_str = """#ifndef LOG_JOB_INFO_H_INCLUDED
#define LOG_JOB_INFO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>


struct log_info{
  char* host;
  char* user;
  char* hostname;
  char* slurm_job_id;
  char* slurm_job_num_nodes;
  char* slurm_job_account;
  char* nodetype;
  int is_compute;
  int uid;
};

void extrct_job_info(struct log_info *job_log);
void reset_job_log();

#endif
"""

	with open("%s.h" % fl_nm, "w") as fl_out:
		fl_out.write(h_str)

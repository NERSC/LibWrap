#ifndef LOG_JOB_INFO_H_INCLUDED
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

void extrct_job_info(struct log_info job_log);
void reset_job_log();

#endif

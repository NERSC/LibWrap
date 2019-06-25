#ifndef LOG_JOB_INFO_H_INCLUDED
#define LOG_JOB_INFO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>
#include <hdf5.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <jansson.h>

extern json_t *root;

struct log_info{
  char* host;
  char* user;
  char* hostname;
  char* slurm_job_id;
  char* slurm_job_num_nodes;
  char* slurm_job_account;
  char* nodetype;
  char* first_hdf5api_time;
  int is_compute;
  int ismpi;
  int uid;
};

void extrct_log_info();
void reset_job_log();

#endif

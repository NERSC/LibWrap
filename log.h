#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>
#include <hdf5.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "my_amqps_sendstring.h"


struct log_info{
  char* host;
  char* user;
  char* hostname;
  char* slurm_job_id;
  char* slurm_job_num_nodes;
  char* slurm_job_account;
  char* nodetype;
  int is_compute;
  char* first_hdf5api_time;
  int ismpi;
  int uid;
};

struct api_counts{
  int open_count;
  int create_count;
  int close_count;
  int dread_count;
  int dwrite_count;
};

extern int open_count, create_count, close_count, dread_count;
extern struct log_info job_log;
extern struct api_counts job_api_counts;

void wrt_log(hid_t result, const char *file_op, const int ismpi);

void do_log(const char *name, hid_t fapl_id, hid_t result,
							const char *file_op);

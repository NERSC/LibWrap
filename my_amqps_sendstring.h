#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <stdint.h>
#include <amqp_ssl_socket.h>
#include <amqp_framing.h>
#include <jansson.h>

#include "utils.h"

extern long long glo_parallel_read_data, glo_parallel_write_data, 
           serial_read_data, serial_write_data;

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

struct api_counts{
  int fcreate_count;
  int fopen_count;
  int fclose_count;
  int dcreate_count;
  int dopen_count;
  int dwrite_count;
  int dread_count;
  int dclose_count;
  int acreate_count;
  int aopen_count;
  int awrite_count;
  int aread_count;
  int aclose_count;
  int gcreate_count;
  int gopen_count;
  int gclose_count;
};

void send_to_mods();

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

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

#include "my_amqps_sendstring.h"
#include "log_job_info.h" /* This should call job_log_info if you want wrapper to extractt default job information like uid*/


extern long long glo_parallel_read_data, glo_parallel_write_data, 
           serial_read_data, serial_write_data;


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

void mpi_finalize_cb();
void log_atexit();

void make_log();
void log_MPI_finalize();
void log_MPI_reduce();

void H5Fcreate_mywrap();
void H5Fopen_mywrap();
void H5Fclose_mywrap();

void H5Acreate2_mywrap();
void H5Aopen_mywrap();
void H5Awrite_mywrap();
void H5Aread_mywrap();
void H5Aclose_mywrap();

void H5Dcreate2_mywrap();
void H5Dopen2_mywrap();
void H5Dwrite_mywrap();
void H5Dread_mywrap();
void H5Dclose_mywrap();

void H5Gcreate2_mywrap();
void H5Gopen2_mywrap();
void H5Gclose_mywrap();

#endif

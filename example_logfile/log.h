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

/* This should call job_log_info*/
#include "log_job_info.h"

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

void make_log();
void log_MPI_finalize();
void log_MPI_reduce();

void H5Fcreate_log();
void H5Fopen_log();
void H5Fclose_log();

void H5Acreate2_log();
void H5Aopen_log();
void H5Awrite_log();
void H5Aread_log();
void H5Aclose_log();

void H5Dcreate2_log();
void H5Dopen2_log();
void H5Dwrite_log();
void H5Dread_log();
void H5Dclose_log();

void H5Gcreate2_log();
void H5Gopen2_log();
void H5Gclose_log();

#endif

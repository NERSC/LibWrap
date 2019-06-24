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

extern struct log_info job_log;
extern struct api_counts serial_api_counts;
extern struct api_counts parallel_api_counts;


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

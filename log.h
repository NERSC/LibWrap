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

void H5Fcreate_log();
void H5Fopen_log();
void H5Fclose_log();
void H5Dread_log();
void H5Dwrite_log();


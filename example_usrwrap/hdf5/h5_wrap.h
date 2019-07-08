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

struct h5_api_counts{ 
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

struct posix_api_counts{
  int read_count;
  int write_count;
};

struct buff_sz{
  long long read;
  long long write;
};

void mpi_finalize_cb();
void log_atexit();

void log_MPI_finalize();
void log_MPI_reduce();

void H5Fcreate_mywrap(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id);
void H5Fopen_mywrap(const char *name, unsigned flags, hid_t fapl_id);
void H5Fclose_mywrap(hid_t file_id);

void H5Acreate2_mywrap(hid_t loc_id, const char *attr_name, hid_t type_id, hid_t space_id,\
							 hid_t acpl_id, hid_t aapl_id);
void H5Aopen_mywrap(hid_t obj_id, const char *attr_name, hid_t aapl_id);
void H5Awrite_mywrap(hid_t attr_id, hid_t mem_type_id, const void *buf);
void H5Aread_mywrap(hid_t attr_id, hid_t mem_type_id, void *buf);
void H5Aclose_mywrap(hid_t attr_id);

void H5Dcreate2_mywrap(hid_t loc_id, const char *name, hid_t dtype_id, hid_t space_id, \
					 hid_t lcpl_id, hid_t dcpl_id, hid_t dapl_id);
void H5Dopen2_mywrap(hid_t loc_id, const char *name, hid_t dapl_id);
void H5Dwrite_mywrap(hid_t dataset_id, hid_t mem_type_id, hid_t mem_space_id, \
		  hid_t file_space_id, hid_t xfer_plist_id, const void * buf);
void H5Dread_mywrap(hid_t dataset_id, hid_t mem_type_id, hid_t mem_space_id, \
			hid_t file_space_id, hid_t xfer_plist_id, void * buf);
void H5Dclose_mywrap(hid_t dataset_id);

void H5Gcreate2_mywrap(hid_t loc_id, const char *name, hid_t lcpl_id, hid_t gcpl_id,\
								 hid_t gapl_id);
void H5Gopen2_mywrap(hid_t loc_id, const char * name, hid_t gapl_id);
void H5Gclose_mywrap(hid_t group_id);

#endif

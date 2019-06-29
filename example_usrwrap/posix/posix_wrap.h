
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <jansson.h>

#include "my_amqps_sendstring.h"
#include "log_job_info.h" /* This should call job_log_info if you want wrapper to extractt default job information like uid*/

void log_atexit();
void write_mywrap(int fd, const void *buf, size_t count);
void read_mywrap(int fd, void *buf, size_t count);

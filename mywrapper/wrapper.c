#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "gotcha/gotcha.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* User included headers*/
#include <hdf5.h>

#include "log_init.h"
#include "h5_wrap.h"

#define NFUNCS 3


pthread_mutex_t log_mutex;



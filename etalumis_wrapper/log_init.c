#include "log_init.h"

int flag_isparallel=0;
static int flag_atexit = 0;

int key_val; /* To add attribute to MPI comm for cb */

void serial_atexit()
{
  if (!flag_isparallel){
    log_atexit(); 
  }
  return ;
}


void _log_init()
{

 if (!flag_atexit)
  {
    atexit(serial_atexit);
    flag_atexit=1;
  }
}

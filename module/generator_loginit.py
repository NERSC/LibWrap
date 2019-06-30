def generate_log_init(fl_nm, str_log_atexit, str_log_MPI_finalize, usr_fl_nm):

	c_str = """#include "log_init.h"

int flag_isparallel=0;
static int flag_atexit = 0;

int key_val; /* To add attribute to MPI comm for cb */

void serial_atexit()
{
  if (!flag_isparallel){
    %s 
  }
  return ;
}
""" % (str_log_atexit )
	if str_log_MPI_finalize:
		mpi_cd_str = \
"""
static int mpi_log_cb(MPI_Comm comm, int keyval, void *attr_val, int *flag)
{
  %s
  return MPI_SUCCESS;
}


void mpi_atexit()
{ 
  int mpi_code;  
  /* add an attribute on MPI_COMM_WORLD to call send_to_mods()
     when it is destroyed, i.e. on MPI_Finalize */
  if(MPI_SUCCESS != (mpi_code = MPI_Comm_create_keyval(MPI_COMM_NULL_COPY_FN, 
  					(MPI_Comm_delete_attr_function *)mpi_log_cb, 
									&key_val, NULL))){
    fprintf(stderr, "MPI_Comm_create_keyval failed");
    return ;
  }
  if(MPI_SUCCESS != (mpi_code = MPI_Comm_set_attr(MPI_COMM_WORLD, key_val, NULL))){
    fprintf(stderr, "MPI_Comm_set_attr failed");
    return ;
  }
  if(MPI_SUCCESS != (mpi_code = MPI_Comm_free_keyval(&key_val))){
    fprintf(stderr, "MPI_Comm_free_keyval failed");
    return ;
  }
  return ;
}


void _log_init()
{ 
  /* If its using MPI call mpi_atexit */
  if (!flag_isparallel)
  {
    int mpi_initialized;
    int mpi_finalized;
 
    MPI_Initialized(&mpi_initialized);
    MPI_Finalized(&mpi_finalized);
    if (mpi_initialized && !mpi_finalized){ 
      mpi_atexit();
      flag_isparallel=1;
      flag_atexit=1;
    }
  }
 
""" % str_log_MPI_finalize
		mpi_incl = "#include <mpi.h>"
	else: 
		mpi_cd_str = \
"""

void _log_init()
{
"""
		mpi_incl = ""
	c_str = "%s%s%s" % (c_str, mpi_cd_str, 
"""
 if (!flag_atexit)
  {
    atexit(serial_atexit);
    flag_atexit=1;
  }
}
""" )
	with open("%s.c"%fl_nm, "w") as fl_out:
		fl_out.write(c_str)
	str_logfl = ""
	if fl_nm:
		str_logfl = "#include \"%s.h\"" % usr_fl_nm
	h_str = """#include <stdlib.h>
#include <stdio.h>
%s
%s

void _log_init();
"""%(mpi_incl, str_logfl)
	with open("%s.h" % fl_nm, "w") as fl_out:
		fl_out.write(h_str);


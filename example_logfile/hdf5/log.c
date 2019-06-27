#include "log.h"

struct api_counts loc_parallel_api_counts, serial_api_counts, glo_parallel_api_counts;

static int tot_serial_api_count = 0;
static int tot_parallel_api_count = 0;

long long loc_parallel_read_data = 0;
long long serial_read_data = 0;
long long loc_parallel_write_data = 0;
long long serial_write_data = 0;
long long glo_parallel_read_data = 0;
long long glo_parallel_write_data = 0;

struct api_counts serial_api_counts;
struct api_counts parallel_api_counts;

json_t *root; 
struct log_info job_log;


void crt_json_log()
{
  root = json_object();
  json_object_set_new(root, "category", json_string("MODS") );
  json_object_set_new(root, "name", json_string("gotchaio-tracer") );
  json_object_set_new(root, "uid", json_integer(job_log.uid) );
  json_object_set_new(root, "nersc host", json_string(job_log.host));
  json_object_set_new(root, "hostname", json_string(job_log.hostname));
  json_object_set_new(root, "user", json_string(job_log.user));
  json_object_set_new(root, "slurm number of nodes", json_string(job_log.slurm_job_num_nodes));
  json_object_set_new(root, "slurm job account", json_string(job_log.slurm_job_account));
  json_object_set_new(root, "nodetype", json_string(job_log.nodetype));
  
  //TODO: JANSSON does not handle unsigned int
  json_object_set_new( root, "category", json_string("MODS") );
  json_object_set_new( root, "name", json_string("gotchaio-tracer") );
  /* Serial Counts */
  json_object_set_new(root, "serial fcreate count", json_integer(serial_api_counts.fcreate_count));
  json_object_set_new(root, "serial fopen count", json_integer(serial_api_counts.fopen_count));
  json_object_set_new(root, "serial acreate count", json_integer(serial_api_counts.acreate_count));
  json_object_set_new(root, "serial aopen count", json_integer(serial_api_counts.aopen_count));
  json_object_set_new(root, "serial aread count", json_integer(serial_api_counts.aread_count));
  json_object_set_new(root, "serial awrite count", json_integer(serial_api_counts.awrite_count));
  json_object_set_new(root, "serial dcreate count", json_integer(serial_api_counts.dcreate_count));
  json_object_set_new(root, "serial dopen count", json_integer(serial_api_counts.dopen_count));
  json_object_set_new(root, "serial dread count", json_integer(serial_api_counts.dread_count));
  json_object_set_new(root, "serial dwrite count", json_integer(serial_api_counts.dwrite_count));
  json_object_set_new(root, "serial gcreate count", json_integer(serial_api_counts.gcreate_count));
  json_object_set_new(root, "serial gopen count", json_integer(serial_api_counts.gopen_count));
  /* Parallel Counts */
  json_object_set_new(root, "parallel fcreate count", json_integer(glo_parallel_api_counts.fcreate_count));
  json_object_set_new(root, "parallel fopen count", json_integer(glo_parallel_api_counts.fopen_count));
  json_object_set_new(root, "parallel dread count", json_integer(glo_parallel_api_counts.dread_count));
  json_object_set_new(root, "parallel dwrite count", json_integer(glo_parallel_api_counts.dwrite_count));
  /* Data Read/Write */ 
  json_object_set_new(root, "total serial dataset read size", json_integer(serial_read_data));
  json_object_set_new(root, "total serial dataset write size", json_integer(serial_write_data));
  json_object_set_new(root, "total parallel dataset read size", json_integer(glo_parallel_read_data));
  json_object_set_new(root, "total parallel dataset write size", json_integer(glo_parallel_write_data));
  
  return ;
}


void reset_api_counts(struct api_counts my_api_counts)
{
  my_api_counts.fcreate_count=0;
  my_api_counts.fopen_count=0;
  my_api_counts.fclose_count=0;
  my_api_counts.acreate_count=0;
  my_api_counts.aopen_count=0;
  my_api_counts.awrite_count=0;
  my_api_counts.aread_count=0;
  my_api_counts.aclose_count=0;
  my_api_counts.dcreate_count=0;
  my_api_counts.dopen_count=0;
  my_api_counts.dwrite_count=0;
  my_api_counts.dread_count=0;
  my_api_counts.dclose_count=0;
  my_api_counts.gcreate_count=0;
  my_api_counts.gopen_count=0;
  my_api_counts.gclose_count=0;
  return ;
}


void log_MPI_reduce()
{
  /* Reduce all of the local sums into the global sum */
  MPI_Reduce(&loc_parallel_api_counts.fopen_count, &glo_parallel_api_counts.fopen_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_parallel_api_counts.fcreate_count, &glo_parallel_api_counts.fcreate_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_parallel_api_counts.dread_count, &glo_parallel_api_counts.dread_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_parallel_api_counts.dwrite_count, &glo_parallel_api_counts.dwrite_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_parallel_api_counts.dopen_count, &glo_parallel_api_counts.dopen_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_parallel_read_data, &glo_parallel_read_data, 1, MPI_INT, MPI_SUM, 
							    0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_parallel_write_data, &glo_parallel_write_data, 1, MPI_INT, MPI_SUM, 
							    0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_parallel_write_data, &glo_parallel_write_data, 1, MPI_INT, MPI_SUM, 
							    0, MPI_COMM_WORLD);
  
  return ;
}

//TODO: atexit should be probably different
void log_atexit()
{
  extrct_job_info(job_log);
  crt_json_log();
  send_to_mods();
  return ;	
}


void mpi_finalize_cb()
{
  MPI_Comm comm = MPI_COMM_WORLD;
  /* Reduce operations */
  log_MPI_reduce();
  int world_rank;
  MPI_Comm_rank(comm, &world_rank);
  if (world_rank == 0)
  {
    extrct_job_info(job_log);
    crt_json_log();
    send_to_mods();
  }
  /* Clean up */
  
  /* To reset counters and free memory */
  tot_parallel_api_count = 0;
  glo_parallel_read_data = 0;
  glo_parallel_write_data = 0;
  loc_parallel_read_data = 0;
  loc_parallel_write_data = 0;

  reset_api_counts(loc_parallel_api_counts);
  reset_api_counts(glo_parallel_api_counts); 
  reset_job_log();  
  return ;	
}


/***************************/
/* User wrappers start here */
/***************************/
hbool_t is_mpi(hid_t fapl_id)
{
  hbool_t have_mpi = false;
  hid_t driver_id;
  // If its the default fapl then we know its serial
  if (fapl_id == H5P_DEFAULT){
    have_mpi = false;
  }
  else if((driver_id = H5Pget_driver(fapl_id)) > 0 && driver_id == H5FD_MPIO){
    have_mpi = true;
  }
  return have_mpi;
}


hbool_t is_mpi_D(hid_t plist_id)
{
  hbool_t have_mpi = false;
  H5FD_mpio_xfer_t xfer_mode = H5FD_MPIO_INDEPENDENT;
  if (plist_id == H5P_DEFAULT){
    have_mpi = false;
  }
  else{
    H5Pget_dxpl_mpio(plist_id, &xfer_mode);
    if(xfer_mode == H5FD_MPIO_INDEPENDENT)
      have_mpi = false;
    else 
      have_mpi = true;
  }
  return have_mpi;
}

void mpi_log(hid_t fapl_id)
{  
  tot_parallel_api_count++;
  return ;
}


void serial_log()
{
  tot_serial_api_count++;
  return ;
}


hsize_t get_dataset_size(void* dset, hid_t mem_type_id, hid_t mem_space_id)
{
  hsize_t r_size;
  hid_t space_id = H5Dget_space((hid_t)dset);  
  if(H5S_ALL == mem_space_id)
    r_size = H5Tget_size(mem_type_id) * (hsize_t)H5Sget_simple_extent_npoints(space_id);
  else
    r_size = H5Tget_size(mem_type_id) * (hsize_t)H5Sget_select_npoints(mem_space_id); 
  return r_size; 
}


void H5Fcreate_log(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id)
{
  if (is_mpi(fapl_id)){
    mpi_log(fapl_id);
    loc_parallel_api_counts.fcreate_count++;
  }
  else{
    serial_log(); 
    serial_api_counts.fcreate_count++;
  }
  return ;
}


void H5Fopen_log(const char *name, unsigned flags, hid_t fapl_id)
{
  if (is_mpi(fapl_id)){
    mpi_log(fapl_id);
    loc_parallel_api_counts.fopen_count++;
  }
  else{
    serial_log();
    serial_api_counts.fopen_count++;
  } 
  return ;
}

/* Assumes serial */
void H5Fclose_log(hid_t attr_id)
{
  serial_log();
  serial_api_counts.fclose_count++;
  return ;
}

/* Assumes serial */
void H5Acreate2_log(hid_t loc_id, const char *attr_name, hid_t type_id, hid_t 
					space_id, hid_t acpl_id, hid_t aapl_id)
{
  serial_log();
  serial_api_counts.acreate_count++;
  return ;
}

/* Assumes serial */
void H5Aopen_log(hid_t obj_id, const char *attr_name, hid_t aapl_id)
{
  serial_log();
  serial_api_counts.aopen_count++;
  return ;
}

/* Assumes serial */
void H5Awrite_log(hid_t attr_id, hid_t mem_type_id, const void *buf)
{
  serial_log();
  serial_api_counts.awrite_count++;
  return ;
}

/* Assumes serial */
void H5Aread_log(hid_t attr_id, hid_t mem_type_id, void *buf)
{
  serial_log();
  serial_api_counts.aread_count++;
  return ;
}

/* Assumes serial */
void H5Aclose_log(hid_t attr_id)
{
  serial_log();
  serial_api_counts.aclose_count++;
  return ;
}

/* Assumes serial */
void H5Dcreate2_log(hid_t loc_id, const char *name, hid_t dtype_id, hid_t space_id,
					 hid_t lcpl_id, hid_t dcpl_id, hid_t dapl_id)
{
  serial_log();
  serial_api_counts.dcreate_count++;
  return ;
}


/* Assumes serial */
void H5Dopen2_log(const char *name, unsigned flags, hid_t dapl_id)
{
  serial_log();
  serial_api_counts.dopen_count++;
  return ;
}


void H5Dwrite_log(void *dset, hid_t mem_type_id, hid_t mem_space_id,
		hid_t file_space_id, hid_t plist_id, void *buf)
{
  hsize_t dwrite_size = get_dataset_size(dset, mem_type_id, mem_space_id);
  if (is_mpi_D(plist_id)){
    tot_parallel_api_count++;
    loc_parallel_api_counts.dwrite_count++;
    loc_parallel_write_data += dwrite_size;
  }
  else{
    tot_serial_api_count++;
    serial_api_counts.dwrite_count++;
    serial_write_data += dwrite_size;
  }
  return ;
}


void H5Dread_log(void *dset, hid_t mem_type_id, hid_t mem_space_id,
		hid_t file_space_id, hid_t plist_id, void *buf)
{
  hsize_t dread_size = get_dataset_size(dset, mem_type_id, mem_space_id);
  if (is_mpi_D(plist_id)){
    tot_parallel_api_count++;
    loc_parallel_api_counts.dread_count++;
    loc_parallel_read_data += dread_size;
  }
  else{
    tot_serial_api_count++;
    serial_api_counts.dread_count++;
    serial_read_data += dread_size;
  }
  return ;
}


void H5Dclose_log(hid_t dataset_id)
{
  serial_log();
  serial_api_counts.dclose_count++;
  return ;
}

/* Assumes serial */
void H5Gcreate2_log(hid_t loc_id, const char *name, hid_t lcpl_id, hid_t gcpl_id, hid_t gapl_id)
{
  serial_log();
  serial_api_counts.gcreate_count++;
  return ;
}

/* Assumes serial */
void H5Gopen2_log(hid_t loc_id, const char * name, hid_t gapl_id)
{
  serial_log();
  serial_api_counts.aopen_count++;
  return ;
}

/* Assumes serial */
void H5Gclose_log(hid_t group_id)
{
  serial_log();
  serial_api_counts.aclose_count++;
  return ;
}



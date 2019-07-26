#include "h5_wrap.h"

long long serial_read_data = 0;
long long serial_write_data = 0;

struct posix_api_counts loc_psx_cnts;
struct posix_api_counts glo_psx_cnts;

struct h5_api_counts loc_serial_api_counts, glo_serial_api_counts;
struct h5_api_counts loc_parallel_api_counts, glo_parallel_api_counts;

struct buff_sz loc_psx_buff_sz, glo_psx_buff_sz;
struct buff_sz loc_ph5_buff_sz, loc_h5_buff_sz;
struct buff_sz glo_ph5_buff_sz, glo_h5_buff_sz;

json_t *json_root; 
struct log_info job_log;


void crt_json_log()
{
  json_root = json_object();
  json_object_set_new(json_root, "category", json_string("MODS") );
  json_object_set_new(json_root, "name", json_string("libwrap-hdf5") );
  json_object_set_new(json_root, "uid", json_integer(job_log.uid) );
  json_object_set_new(json_root, "nersc host", json_string(job_log.host));
  json_object_set_new(json_root, "hostname", json_string(job_log.hostname));
  json_object_set_new(json_root, "user", json_string(job_log.user));
  json_object_set_new(json_root, "slurm number of nodes", json_string(job_log.slurm_job_num_nodes));
  json_object_set_new(json_root, "slurm job account", json_string(job_log.slurm_job_account));
  json_object_set_new(json_root, "nodetype", json_string(job_log.nodetype));
  json_object_set_new(json_root, "is_compute_node", json_integer(job_log.is_compute));
  json_object_set_new(json_root, "slurm job id", json_string(job_log.slurm_job_id));
  
  //TODO: JANSSON does not handle unsigned int
  /* Serial Counts */
  json_object_set_new(json_root, "serial fcreate count", json_integer(glo_serial_api_counts.fcreate_count));
  json_object_set_new(json_root, "serial fopen count", json_integer(glo_serial_api_counts.fopen_count));
  json_object_set_new(json_root, "serial acreate count", json_integer(glo_serial_api_counts.acreate_count));
  json_object_set_new(json_root, "serial aopen count", json_integer(glo_serial_api_counts.aopen_count));
  json_object_set_new(json_root, "serial aread count", json_integer(glo_serial_api_counts.aread_count));
  json_object_set_new(json_root, "serial awrite count", json_integer(glo_serial_api_counts.awrite_count));
  json_object_set_new(json_root, "serial dcreate count", json_integer(glo_serial_api_counts.dcreate_count));
  json_object_set_new(json_root, "serial dopen count", json_integer(glo_serial_api_counts.dopen_count));
  json_object_set_new(json_root, "serial dread count", json_integer(glo_serial_api_counts.dread_count));
  json_object_set_new(json_root, "serial dwrite count", json_integer(glo_serial_api_counts.dwrite_count));
  json_object_set_new(json_root, "serial gcreate count", json_integer(glo_serial_api_counts.gcreate_count));
  json_object_set_new(json_root, "serial gopen count", json_integer(glo_serial_api_counts.gopen_count));
  json_object_set_new(json_root, "posix read count", json_integer(glo_psx_cnts.read_count));
  json_object_set_new(json_root, "posix write count", json_integer(glo_psx_cnts.write_count));
 
  /* Parallel Counts */
  json_object_set_new(json_root, "parallel fcreate count", json_integer(glo_parallel_api_counts.fcreate_count));
  json_object_set_new(json_root, "parallel fopen count", json_integer(glo_parallel_api_counts.fopen_count));
  json_object_set_new(json_root, "parallel dread count", json_integer(glo_parallel_api_counts.dread_count));
  json_object_set_new(json_root, "parallel dwrite count", json_integer(glo_parallel_api_counts.dwrite_count));

  /* Data Read/Write */ 
  json_object_set_new(json_root, "serial hdf5 dataset read size", json_integer(glo_h5_buff_sz.read));
  json_object_set_new(json_root, "serial hdf5 dataset write size", json_integer(glo_h5_buff_sz.write));
  json_object_set_new(json_root, "parallel hdf5 dataset read size", json_integer(glo_ph5_buff_sz.read));
  json_object_set_new(json_root, "parallel hdf5 dataset write size", json_integer(glo_ph5_buff_sz.write));
  json_object_set_new(json_root, "posix dataset read size", json_integer(glo_psx_buff_sz.read));
  json_object_set_new(json_root, "posix dataset write size", json_integer(glo_psx_buff_sz.write));
 
  return ;
}


void reset_api_counts(struct h5_api_counts my_api_counts)
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
  /* Parallel api counts */
  MPI_Reduce(&loc_parallel_api_counts.fopen_count, &glo_parallel_api_counts.fopen_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_parallel_api_counts.fcreate_count, &glo_parallel_api_counts.fcreate_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_parallel_api_counts.dread_count, &glo_parallel_api_counts.dread_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_parallel_api_counts.dwrite_count, &glo_parallel_api_counts.dwrite_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  /* Serial api counts */ 
  MPI_Reduce(&loc_serial_api_counts.acreate_count, &glo_serial_api_counts.acreate_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_serial_api_counts.aopen_count, &glo_serial_api_counts.aopen_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_serial_api_counts.aread_count, &glo_serial_api_counts.aread_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_serial_api_counts.awrite_count, &glo_serial_api_counts.awrite_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  
  MPI_Reduce(&loc_serial_api_counts.dopen_count, &glo_serial_api_counts.dopen_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_serial_api_counts.dcreate_count, &glo_serial_api_counts.dcreate_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  MPI_Reduce(&loc_serial_api_counts.gopen_count, &glo_serial_api_counts.gopen_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_serial_api_counts.gcreate_count, &glo_serial_api_counts.gcreate_count, 
					1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
 
  /* Data Read/Write */
  MPI_Reduce(&loc_ph5_buff_sz.read, &glo_ph5_buff_sz.read, 1, MPI_LONG_LONG, MPI_SUM, 
							    0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_ph5_buff_sz.write, &glo_ph5_buff_sz.write, 1, MPI_LONG_LONG, MPI_SUM, 
							    0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_h5_buff_sz.read, &glo_h5_buff_sz.read, 1, MPI_LONG_LONG, MPI_SUM, 
							    0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_h5_buff_sz.write, &glo_h5_buff_sz.write, 1, MPI_LONG_LONG, MPI_SUM, 
							    0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_psx_buff_sz.read, &glo_psx_buff_sz.read, 1, MPI_LONG_LONG, MPI_SUM, 
							    0, MPI_COMM_WORLD);
  MPI_Reduce(&loc_psx_buff_sz.write, &glo_psx_buff_sz.write, 1, MPI_LONG_LONG, MPI_SUM, 
							    0, MPI_COMM_WORLD);
  return ;
}


void serial_reduce()
{
  glo_psx_cnts = loc_psx_cnts;
  glo_psx_buff_sz = loc_psx_buff_sz;
  glo_h5_buff_sz = loc_h5_buff_sz;
  glo_serial_api_counts = loc_serial_api_counts;
}

//TODO: atexit should be probably different
void log_atexit()
{
  extrct_job_info(&job_log);
  serial_reduce();
  crt_json_log();
  send_to_mods(json_root);
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
    extrct_job_info(&job_log);
    crt_json_log();
    send_to_mods(json_root);
  }
  /* Clean up */
  
  /* To reset counters and free memory */
  glo_ph5_buff_sz.read = 0;
  glo_ph5_buff_sz.write = 0;
  loc_ph5_buff_sz.read = 0; 
  loc_ph5_buff_sz.write = 0;

  reset_api_counts(loc_parallel_api_counts);
  reset_api_counts(glo_parallel_api_counts); 
  reset_job_log();  
  return ;	
}


/***************************/
/* User wrappers start here */
/***************************/
/* Determines if the the api call is collective */
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

/* Determines if the Dread/Dwrite are collective */
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

/* Gets the datasize written/read */
hsize_t get_dataset_size(hid_t dset, hid_t mem_type_id, hid_t mem_space_id)
{
  hsize_t r_size;
  hid_t space_id = H5Dget_space((hid_t)dset);  
  if(H5S_ALL == mem_space_id)
    r_size = H5Tget_size(mem_type_id) * (hsize_t)H5Sget_simple_extent_npoints(space_id);
  else
    r_size = H5Tget_size(mem_type_id) * (hsize_t)H5Sget_select_npoints(mem_space_id); 
  return r_size; 
}


void write_mywrap(int fd, const void *buf, size_t count)
{
  loc_psx_cnts.write_count++;
  loc_psx_buff_sz.write+=(long long)count;
  return ;
}


void read_mywrap(int fd, void *buf, size_t count)
{
  loc_psx_cnts.read_count++;
  loc_psx_buff_sz.read+=(long long)count;
  return ;
}


void H5Fcreate_mywrap(const char *name, unsigned flags, hid_t fcpl_id, hid_t fapl_id)
{
  if (is_mpi(fapl_id)){
    loc_parallel_api_counts.fcreate_count++;
  }
  else{
    loc_serial_api_counts.fcreate_count++;
  }
  return ;
}


void H5Fopen_mywrap(const char *name, unsigned flags, hid_t fapl_id)
{
  if (is_mpi(fapl_id)){
    loc_parallel_api_counts.fopen_count++;
  }
  else{
    loc_serial_api_counts.fopen_count++;
  } 
  return ;
}

/* Assumes serial */
void H5Fclose_mywrap(hid_t attr_id)
{
  loc_serial_api_counts.fclose_count++;
  return ;
}

/* Assumes serial */
void H5Acreate2_mywrap(hid_t loc_id, const char *attr_name, hid_t type_id, hid_t 
					space_id, hid_t acpl_id, hid_t aapl_id)
{
  loc_serial_api_counts.acreate_count++;
  return ;
}

/* Assumes serial */
void H5Aopen_mywrap(hid_t obj_id, const char *attr_name, hid_t aapl_id)
{
  loc_serial_api_counts.aopen_count++;
  return ;
}

/* Assumes serial */
void H5Awrite_mywrap(hid_t attr_id, hid_t mem_type_id, const void *buf)
{
  loc_serial_api_counts.awrite_count++;
  return ;
}

/* Assumes serial */
void H5Aread_mywrap(hid_t attr_id, hid_t mem_type_id, void *buf)
{
  loc_serial_api_counts.aread_count++;
  return ;
}

/* Assumes serial */
void H5Aclose_mywrap(hid_t attr_id)
{
  loc_serial_api_counts.aclose_count++;
  return ;
}

/* Assumes serial */
void H5Dcreate2_mywrap(hid_t loc_id, const char *name, hid_t dtype_id, hid_t space_id,
					 hid_t lcpl_id, hid_t dcpl_id, hid_t dapl_id)
{
  loc_serial_api_counts.dcreate_count++;
  return ;
}


/* Assumes serial */
void H5Dopen2_mywrap(const char *name, unsigned flags, hid_t dapl_id)
{
  loc_serial_api_counts.dopen_count++;
  return ;
}


void H5Dwrite_mywrap(hid_t dataset_id, hid_t mem_type_id, hid_t mem_space_id, 
		  hid_t file_space_id, hid_t xfer_plist_id, const void * buf)
{
  hsize_t dwrite_size = get_dataset_size(dataset_id, mem_type_id, mem_space_id);
  if (is_mpi_D(xfer_plist_id)){
    loc_parallel_api_counts.dwrite_count++;
    loc_ph5_buff_sz.write += dwrite_size;
  }
  else{
    loc_serial_api_counts.dwrite_count++;
    loc_h5_buff_sz.read += dwrite_size;
  }
  return ;
}


void H5Dread_mywrap(hid_t dataset_id, hid_t mem_type_id, hid_t mem_space_id, 
			hid_t file_space_id, hid_t xfer_plist_id, void * buf)
{
  hsize_t dread_size = get_dataset_size(dataset_id, mem_type_id, mem_space_id);
  if (is_mpi_D(xfer_plist_id)){
    loc_parallel_api_counts.dread_count++;
    loc_ph5_buff_sz.read += dread_size;
  }
  else{
    loc_serial_api_counts.dread_count++;
    loc_h5_buff_sz.write += dread_size;
  }
  return ;
}


void H5Dclose_mywrap(hid_t dataset_id)
{
  loc_serial_api_counts.dclose_count++;
  return ;
}

/* Assumes serial */
void H5Gcreate2_mywrap(hid_t loc_id, const char *name, hid_t lcpl_id, hid_t gcpl_id, hid_t gapl_id)
{
  loc_serial_api_counts.gcreate_count++;
  return ;
}

/* Assumes serial */
void H5Gopen2_mywrap(hid_t loc_id, const char * name, hid_t gapl_id)
{
  loc_serial_api_counts.aopen_count++;
  return ;
}

/* Assumes serial */
void H5Gclose_mywrap(hid_t group_id)
{
  loc_serial_api_counts.aclose_count++;
  return ;
}


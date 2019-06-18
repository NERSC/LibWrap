#include "log.h"

struct log_info job_log; 
struct api_counts loc_parallel_api_counts, serial_api_counts, glo_parallel_api_counts;

static int tot_serial_api_count = 0;
static int tot_parallel_api_count = 0;

// CR note: int might overflow
loc_parallel_read_data = 0;
serial_read_data = 0;
loc_parallel_write_data = 0;
serial_write_data = 0;
glo_parallel_read_data = 0;
glo_parallel_write_data = 0;


// To add attribute to MPI comm for cb
int key_val;


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


void extrct_log_info(const int ismpi)
{
  unsigned long long ts = (unsigned long)time(NULL);
  time_t rtime;
  time(&rtime);
  struct tm * info = localtime(&rtime);
   
  job_log.ismpi = ismpi;
  job_log.uid = getuid(); 
  job_log.first_hdf5api_time = asctime(info); 
  job_log.host = getenv("NERSC_HOST");
  job_log.user = getenv("USER");
  //TODO: This seems to take time. check.
  //char hostnamebuffer[1024];
  //gethostname(hostnamebuffer, sizeof(hostnamebuffer));
  //puts(hostnamebuffer);
  //strcpy(job_log.hostname, hostnamebuffer);
  
  //job_log.slurm_job_id = ()
  job_log.slurm_job_num_nodes = getenv("SLURM_JOB_NUM_NODES");
  job_log.slurm_job_account = getenv("SLURM_JOB_ACCOUNT");
  
  // This is part to get the nodetype
  job_log.nodetype="None";  
  if (strcmp(job_log.host, "cori")==0){
    FILE *fptr = fopen("/proc/cpuinfo", "r");
    if (fptr == NULL)
      fprintf(stderr, "failed to open /proc/cpuinfo\n");
    char* line=NULL;
    size_t len;
    ssize_t read;
    while ((read=getline(&line, &len, fptr))!=-1){
      char tempstr[10];
      strncpy(tempstr, line, 10);
      if (strcmp(tempstr, "model name")==0){
        if (strstr(line, "Phi")!=NULL)
          job_log.nodetype = "KNL";
        else {
          job_log.nodetype = "Haswell";
        }
        break; 
      }
    }
    fclose(fptr);
  }
  //TODO: get this working
  /*  
  if (strcmp(job_log.hostname, "nid")==0)
    job_log.is_compute = 1;
  else
    job_log.is_compute = 0; 
  */
  return ;
}


void reset_api_counts(struct api_counts my_api_counts)
{
  my_api_counts.fopen_count = 0;
  my_api_counts.fcreate_count = 0; 
  my_api_counts.fclose_count = 0;
  my_api_counts.dread_count = 0;
  my_api_counts.dwrite_count = 0;
  return ;
}


static int mpi_send_mods_cb(MPI_Comm comm, int keyval, void *attr_val, int *flag)
{

  // Reduce all of the local sums into the global sum
  MPI_Reduce(&loc_parallel_api_counts.fopen_count, &glo_parallel_api_counts.fopen_count, 
					1, MPI_INT, MPI_MAX, 0, comm);
  MPI_Reduce(&loc_parallel_api_counts.dread_count, &glo_parallel_api_counts.dread_count, 
					1, MPI_INT, MPI_MAX, 0, comm);
  MPI_Reduce(&loc_parallel_api_counts.dwrite_count, &glo_parallel_api_counts.dwrite_count, 
					1, MPI_INT, MPI_MAX, 0, comm);
  MPI_Reduce(&loc_parallel_read_data, &glo_parallel_read_data, 1, MPI_INT, MPI_SUM, 
							    0, comm);
  MPI_Reduce(&loc_parallel_write_data, &glo_parallel_write_data, 1, MPI_INT, MPI_SUM, 
							    0, comm);
  
  int world_rank;
  MPI_Comm_rank(comm, &world_rank);
  if(world_rank==0){
    send_to_mods();
    //fprintf(stderr, "Global sum for process %d - %d\n",
    //   world_rank, glo_parallel_api_counts.fopen_count);
    //fprintf(stderr, "Local sum for process %d - %d\n",
    //     world_rank, loc_parallel_api_counts.fopen_count);
  }
  
  // Reset all counts
  tot_parallel_api_count = 0;
  glo_parallel_read_data = 0;
  glo_parallel_write_data = 0;
  loc_parallel_read_data = 0;
  loc_parallel_write_data = 0;

  reset_api_counts(loc_parallel_api_counts);
  reset_api_counts(glo_parallel_api_counts); 
  // Reset log information
  job_log.host = NULL;
  job_log.user = NULL;
  job_log.hostname = NULL;
  job_log.slurm_job_id = NULL;
  job_log.slurm_job_num_nodes = NULL;
  job_log.slurm_job_account = NULL;
  job_log.nodetype = NULL;
  job_log.is_compute = -1;
  job_log.first_hdf5api_time = NULL;
  job_log.ismpi = -1;
  job_log.uid = -1;
  
  return MPI_SUCCESS;
}


void mpi_atexit()
{ 
  int mpi_initialized;
  int mpi_finalized;
  int mpi_code;
  
  MPI_Initialized(&mpi_initialized);
  MPI_Finalized(&mpi_finalized);
   
  /* add an attribute on MPI_COMM_WORLD to call send_to_mods()
     when it is destroyed, i.e. on MPI_Finalize */
  if (mpi_initialized && !mpi_finalized) {
      if(MPI_SUCCESS != (mpi_code = MPI_Comm_create_keyval(MPI_COMM_NULL_COPY_FN, 
  					(MPI_Comm_delete_attr_function *)mpi_send_mods_cb, 
									&key_val, NULL))){
  	//HMPI_GOTO_ERROR(FAIL, "MPI_Comm_create_keyval failed", mpi_code)
        fprintf(stderr, "MPI_Comm_create_keyval failed");
        return ;
      }
      if(MPI_SUCCESS != (mpi_code = MPI_Comm_set_attr(MPI_COMM_WORLD, key_val, NULL))){
  	//HMPI_GOTO_ERROR(FAIL, "MPI_Comm_set_attr failed", mpi_code)
        fprintf(stderr, "MPI_Comm_set_attr failed");
        return ;
      }
      if(MPI_SUCCESS != (mpi_code = MPI_Comm_free_keyval(&key_val))){
        //HMPI_GOTO_ERROR(FAIL, "MPI_Comm_free_keyval failed", mpi_code)
        fprintf(stderr, "MPI_Comm_free_keyval failed");
        return ;
      }
  }
  return ;
}

// CR Note- Do we need get_fapl_mpio? 
void mpi_extrct_log_info(hid_t fapl_id)
{
  int world_rank;
  MPI_Comm mpi_comm = MPI_COMM_WORLD;
 
  // Fetch the comm
  /*
  MPI_Comm mpi_comm = MPI_COMM_NULL;
  MPI_Info mpi_info = MPI_COMM_INFO;
  MPI_Comm_size(mpi_comm, &world_size);
  H5Pget_fapl_mpio(fapl_id, &mpi_comm, &mpi_info);
  */
  MPI_Comm_rank(mpi_comm, &world_rank);
  
  // Write from only one rank per communicator
  if (world_rank == 0)
    extrct_log_info(1);
  return ;
}


void mpi_log(hid_t fapl_id)
{
  if (tot_parallel_api_count == 0){
    mpi_extrct_log_info(fapl_id);
    mpi_atexit();
  }
  tot_parallel_api_count++;
  return ;
}


void serial_log()
{
  if (tot_serial_api_count == 0){
    extrct_log_info(0);
    atexit(send_to_mods);
  }
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

//FIXME: H5get_fapl_mpio does not work
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


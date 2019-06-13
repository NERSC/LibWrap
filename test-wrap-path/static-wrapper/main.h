struct log_info{
  char* host;
  char* user;
  char* hostname;
  char* slurm_job_id;
  char* slurm_job_num_nodes;
  char* slurm_job_account;
  char* nodetype;
  int is_compute;
  char* first_hdf5api_time;
  int ismpi;
  int uid;
};


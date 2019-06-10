/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 *  This example illustrates how to create a dataset that is a 4 x 6 
 *  array.  It is used in the HDF5 Tutorial.
 */

#include "hdf5.h"
#include "mpi.h"
#define FILE "p_dset.h5"


//TODO create with only one rank 
int main() {

   hid_t       file_id, dataset_id, dataspace_id, plist_id;  /* identifiers */
   hsize_t     dims[2];
   herr_t      status;
   
   MPI_Init(NULL, NULL);
    
   /* Set the file access templace for parallel IO. */
   plist_id = H5Pcreate(H5P_FILE_ACCESS);
   
   /* tell HDF5 library that we want to use MPI-IO for writing. */
   H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);

   /* Create a new file using default properties. */
   file_id = H5Fcreate(FILE, H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);   
   
   /* Create the data space for the dataset. */
   dims[0] = 4; 
   dims[1] = 6; 
   dataspace_id = H5Screate_simple(2, dims, NULL);
   
   /* Create the dataset. */
   dataset_id = H5Dcreate2(file_id, "/dset", H5T_STD_I32BE, dataspace_id, 
                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

   
   /* Release the file access template. */
   H5Pclose(plist_id);

   /* End access to the dataset and release resources used by it. */
   status = H5Dclose(dataset_id);

   /* Terminate access to the data space. */ 
   status = H5Sclose(dataspace_id);

   /* Close the file. */
   status = H5Fclose(file_id);
   
   /* Exit MPI. */
   MPI_Finalize();
}


#!/bin/bash
#SBATCH -J GOTCHA-IO-test_p
#SBATCH -N 2
#SBATCH -C haswell
#SBATCH -q regular
#SBATCH --time=00:05:00
#SBATCH -o out_sbatch_test_p 

cd $HOME/projectcode/gotcha-io/test
GOTCHA=$HOME/projectcode/GOTCHA/install/
module swap PrgEnv-gnu PrgEnv-intel
module swap cray-hdf5 cray-hdf5-parallel
module load openssl/1.1.0a
export LD_LIBRARY_PATH=/usr/common/software/rabbitmq/0.9.0/lib64:$LD_LIBRARY_PATH 
export JANSSON=$HOME/projectcode/jansson-2.12
#And finally run the job
#srun ./test_p.sh
GOTCHAH5=../lgotchah5.so
GOTCHALIB=${GOTCHA}/lib64/libgotcha.so
export LD_PRELOAD=$GOTCHAH5:$GOTCHALIB
srun ./ph5example

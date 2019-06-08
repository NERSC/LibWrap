#!/bin/bash
#SBATCH -J GOTCHA-IO-test_p
#SBATCH -N 2
#SBATCH -C knl
#SBATCH -q regular
#SBATCH --time=00:05:00
#SBATCH -o out_sbatch_test_p 

cd $HOME/projectcode/gotcha-io/test
GOTCHA=$HOME/projectcode/GOTCHA/install/
module swap cray-hdf5 cray-hdf5-parallel
#And finally run the job
#srun ./test_p.sh
GOTCHAH5=../lgotchah5.so
GOTCHALIB=${GOTCHA}/lib64/libgotcha.so
export LD_PRELOAD=$GOTCHAH5:$GOTCHALIB
srun ./ph5example

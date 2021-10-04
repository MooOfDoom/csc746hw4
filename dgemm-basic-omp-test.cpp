#!/bin/bash -l
#SBATCH --constraint=knl
#SBATCH --cpus-per-task=1
#SBATCH --qos=debug
#SBATCH --nodes=1
#SBATCH --time=00:04:00
#SBATCH --cpu-freq=1400000
#SBATCH --job-name=job-@BENCHMARK@
#SBATCH --output=job-@BENCHMARK@.o%j
#SBATCH --error=job-@BENCHMARK@.e%j

# set some openmp variables: 
# OMP_PLACES=threads maps OpenMP threads to hardware threads
# OMP_PROC_BIND=spread binds threads as evenly as possible
#
# see https://docs.nersc.gov/jobs/affinity/ for more information

export OMP_PLACES=threads
export OMP_PROC_BIND=spread

# use PERF_COUNTER_GROUP to tell likwif-perfctr which perf counter group to monitor
# the default value here, HBM_CACHE, exists on Cori's KNL nodes, but may not be present
# on other processors
export PERF_COUNTER_GROUP=HBM_CACHE

# use the MARKER_FLAG variable to activate LIKWID's marker API, which will result in 
# perf counter data being collected only in the region of code surrounded by marker start/stop
# calls. If MARKER_FLAG is not defined, then the LIKWID marker API will not be activated,
# and the resulting performance data reflects the run of the entire program
export MARKER_FLAG="-m"

# iterate over some number of threads. You will need to modify this for HW4 to reflect
# the levels of concurrency in the assignment

# Note: in HW4, we are NOT using OMP_NUM_THREADS to tell your program and OpenMP the
# concurrency level. instead, we tell likwid-perfctr the concurrency level,
# and then likwid-perfctr manages setting OMP_NUM_THREADS

# For the BMMCO code, you MUST make two modifications to the job-blocked-omp script that
# cmake generates from job.in:
# 1. uncomment the for loop that iterates over block sizes: for B in 4 16 63, and uncomment
#    the following line containing "do", and then also uncomment the line containing the
#    closing "done # iterating over block size".
# 2. modify the srun line where you run your program to add a -B $B to the end of the argument list,
# for example:
#   srun -n 1 likwid-perfctr $MARKER_FLAG -g $PERF_COUNTER_GROUP -C N:0-$maxcore ./benchmark-@BENCHMARK@ -N $N
# becomes
#   srun -n 1 likwid-perfctr $MARKER_FLAG -g $PERF_COUNTER_GROUP -C N:0-$maxcore ./benchmark-@BENCHMARK@ -N $N -B $B
#

for N in 128 # loop over problem sizes
   do
   
   for t in 16 # loop over concurrency level
      do
      
      let maxcore=$t-1
      echo srun -n 1 likwid-perfctr $MARKER_FLAG -g $PERF_COUNTER_GROUP -C N:0-$maxcore ./benchmark-@BENCHMARK@ -N $N 
      srun -n 1 likwid-perfctr $MARKER_FLAG -g $PERF_COUNTER_GROUP -C N:0-$maxcore ./benchmark-@BENCHMARK@ -N $N 

   done # iterate over concurrency level
done # iterate over problem size

# eof


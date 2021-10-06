# mmul-omp instructional test harness

This directory contains a benchmark harness for testing different OpenMP-enabled parallel
implementations of matrix multiply for varying problem sizes and, for the blocked mmul 
implmentation, at varying block sizes. The harness includes a benchmark for a reference
implementation that uses CBLAS dgemm for matrix-matrix multiply. 

The main code is benchmark.cpp, which sets up the problem, iterates over problem
(and block) sizes, sets up the matrices, executes the mmul call, and tests the
result for accuracy by comparing your result against a reference implementation (CBLAS).

The benchmark.cpp code also makes calls to LIKWID's Marker API. You will need to add
LIKWID Marker API instrumentation to your implementation of basic and blocked matrix
multiply. There are some comments in dgemm-basic-omp.cpp and dgemm-blocked-omp.cpp
to remind you of this fact. See dgemm-blas.cpp for example usage of the per-thread
LIKWID Marker API, which is valid even if running a serial code.

Note that cmake needs to be able to find the CBLAS package. For CSC 746 Fall 2021,
this condition is true on Cori@NERSC and on the class VM. It is also true for some
other platforms, but you are on your own if using a platform other than Cori@NERSC
or the class VM.

Cmake also needs to be able to find LIKWID. This condition is satisfied on Cori when
you first load the likwid/5.2.0 module, or on the VM when you load the likwid-5.2.0
module:  
* module load likwid/5.2.0   # on cori  
* module load likwid-5.2.0   # on the VM


# Build instructions - Cori

When building on Cori, make sure you are on a KNL node when doing the compilation. The
Cori login nodes are *not* KNL nodes, the Cori login nodes have Intel Xeon E5-2698
processors, not the Intel Xeon Phi 7250 processors.  The simplest way to do this is
grab an interactive KNL node:

% salloc --nodes 1 --qos interactive --time 01:00:00 --constraint knl --perf likwid --account m3930

Load the correct environment modules:

% module swap PrgEnv-intel PrgEnv-gnu
% module load cmake
% module load likwid/5.2.0

After downloading, cd into the main source directly, then:

% mkdir build  
% cd build  
% cmake ../  -Wno-dev
% make

# Platforms

Due to the dependency on LIKWID and LIKWID's dependency on the Linux MSR kernel
module, the best bet for this assignment is to use Cori@NERSC.

On the VM, the codes will compile and run with likwid-perfctr in serial, but parallel
runs will fail.

Other Linux platforms are possible, but you are on your own to get LIKWID installed,
built, and working.

# Changes to benchmark.cpp from HW2

This version of benchmark.cpp is modified to accept two command line arguments:

* -N nn   # sets the problem size  
* -B bb   # sets the block size

This change was made to make it possible to control these parameters from the command
line, which is handy for scripting up the test suite.

# Changes to benchmark.cpp from HW2

This version of benchmark.cpp is modified to accept two command line arguments:

* -N nn   # sets the problem size  
* -B bb   # sets the block size

This change was made to make it possible to control these parameters from the command
line, which is handy for scripting up the test suite.

# Changes to benchmark.cpp from HW2

This version of benchmark.cpp is modified to accept two command line arguments:

* -N nn   # sets the problem size  
* -B bb   # sets the block size

This change was made to make it possible to control these parameters from the command
line, which is handy for scripting up the test suite.

# Changes to benchmark.cpp from HW2

This version of benchmark.cpp is modified to accept two command line arguments:

* -N nn   # sets the problem size  
* -B bb   # sets the block size

This change was made to make it possible to control these parameters from the command
line, which is handy for scripting up the test suite.

# Running the benchmarks

When you run cmake, it generates three bash script files that you may use on Cori to
run the test battery for HW4.

## Requesting specific  hardware performance counters

All configurations will require modification to set the specific LIKWID hardware performance
counter group you want to collect. 

The default group is PERF_COUNTER_GROUP=HBM_CACHE, which
is documented here: https://github.com/RRZE-HPC/likwid/blob/master/groups/knl/HBM_CACHE.txt

If you want to collect different hardware performance counters, replace HBM_CACHE with the
name of the performance counter group you want to collect. likwid-perfctr -a will give
you a list of all the supported performance counter groups on the platform.

##  Running the codes

After running cmake, the job scripts should already be setup as needed. 

* job-basic-omp : will run Basic MM over the problem sizes and OpenMP concurrency levels requested by HW4.

* job-blas : will run CBLAS over the problem sizes requested by HW4.

* job-blocked-omp: will run BMMCO over the problem sizes, block sizes, and OpenMP concurrency levels requested by HW4.

* job-basic-omp-test : will run Basic MM over one problem size and OpenMP concurrency level.

* job-blas-test : will run CBLAS over one problem size.

* job-blocked-omp-test : will run BMMCO over one problem size, block size, and OpenMP concurrency level. 

* job-basic-omp-noperf : will run Basic MM over the problem sizes and OpenMP concurrency levels requested by HW4 without likwid-perfctr.

* job-blas-noperf : will run CBLAS over the problem sizes requested by HW4 without likwid-perfctr.

* job-blocked-omp-noperf: will run BMMCO over the problem sizes, block sizes, and OpenMP concurrency levels requested by HW4 without likwid-perfctr.

The job files can be run with (for instance)

% sbatch -t 30 job-basic-omp

to request up to 30 minutes of run time. The job will run and write its timing output to job--basic-omp.oXXXXXXXX, where XXXXXXXX is the job number that was assigned by the sbatch.

#eof

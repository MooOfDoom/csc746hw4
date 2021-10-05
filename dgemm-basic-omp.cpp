#include <iostream>
#include <omp.h>
#include "likwid-stuff.h"

const char* dgemm_desc = "Basic implementation, OpenMP-enabled, three-loop dgemm.";

/*
 * This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are n-by-n matrices stored in column-major format.
 * On exit, A and B maintain their input values.
 */
void square_dgemm(int n, double* A, double* B, double* C) 
{
   // insert your code here: implementation of basic matrix multiply with OpenMP parallelism enabled

   // be sure to include LIKWID_MARKER_START(MY_MARKER_REGION_NAME) inside the block of parallel code,
   // but before your matrix multiply code, and then include LIKWID_MARKER_STOP(MY_MARKER_REGION_NAME)
   // after the matrix multiply code but before the end of the parallel code block.
   // implementation of basic matrix multiply
   
   #pragma omp parallel
   {
#ifdef LIKWID_PERFMON
      LIKWID_MARKER_START(MY_MARKER_REGION_NAME);
#endif
      int nthreads = omp_get_num_threads();
      int thread_id = omp_get_thread_num();
      int total = n/nthreads;
      int end = total*(thread_id + 1);
      for (int j = total*thread_id; j < end; ++j)
      {
         for (int k = 0; k < n; ++k)
         {
            for (int i = 0; i < n; ++i)
            {
               C[j*n + i] += A[k*n + i]*B[j*n + k];
            }
         }
      }
#ifdef LIKWID_PERFMON
      LIKWID_MARKER_STOP(MY_MARKER_REGION_NAME);
#endif
   }
}

/*
 * Used to warmup the unit, so we measure typical timings, not including
 * one time initialization.
 */
void square_dgemm_warmup(int n, double* A, double* B, double* C)
{
   #pragma omp parallel
   {
      #pragma omp for
      for (int j = 0; j < n; ++j)
      {
         for (int k = 0; k < n; ++k)
         {
            for (int i = 0; i < n; ++i)
            {
               C[j*n + i] += A[k*n + i]*B[j*n + k];
            }
         }
      }
   }
}

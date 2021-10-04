#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include "likwid-stuff.h"

const char* dgemm_desc = "Blocked dgemm, OpenMP-enabled";


/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are n-by-n matrices stored in column-major format.
 * On exit, A and B maintain their input values. */
void square_dgemm_blocked(int n, int block_size, double* A, double* B, double* C) 
{
   // insert your code here: implementation of blocked matrix multiply with copy optimization and OpenMP parallelism enabled

   // be sure to include LIKWID_MARKER_START(MY_MARKER_REGION_NAME) inside the block of parallel code,
   // but before your matrix multiply code, and then include LIKWID_MARKER_STOP(MY_MARKER_REGION_NAME)
   // after the matrix multiply code but before the end of the parallel code block.
   
   #pragma omp parallel
   {
#ifdef LIKWID_PERFMON
      LIKWID_MARKER_START(MY_MARKER_REGION_NAME);
#endif
      double* a_block = new double[block_size*block_size];
      double* b_block = new double[block_size*block_size];
      double* c_block = new double[block_size*block_size];
      
      int n_blocks = n / block_size;
      #pragma omp for
      for (int block_i = 0; block_i < n_blocks; ++ block_i)
      {
         for (int block_j = 0; block_j < n_blocks; ++block_j)
         {
            // copy the current block of C into c_block
            for (int j = 0; j < block_size; ++j)
            {
               for (int i = 0; i < block_size; ++i)
               {
                  c_block[j*block_size + i] = C[(block_j*block_size + j)*n + block_i*block_size + i];
               }
            }
            
            for (int block_k = 0; block_k < n_blocks; ++block_k)
            {
               // copy the current block of A and B into a_block and b_block
               for (int j = 0; j < block_size; ++j)
               {
                  for (int i = 0; i < block_size; ++i)
                  {
                     a_block[j*block_size + i] = A[(block_k*block_size + j)*n + block_i*block_size + i];
                     b_block[j*block_size + i] = B[(block_j*block_size + j)*n + block_k*block_size + i];
                  }
               }
               
               for (int j = 0; j < block_size; ++j)
               {
                  for (int k = 0; k < block_size; ++k)
                  {
                     for (int i = 0; i < block_size; ++i)
                     {
                        c_block[j*block_size + i] +=
                           a_block[k*block_size + i]*b_block[j*block_size + k];
                     }
                  }
               }
            }
            
            // copy c_block back to the current block of C
            for (int j = 0; j < block_size; ++j)
            {
               for (int i = 0; i < block_size; ++i)
               {
                  C[(block_j*block_size + j)*n + block_i*block_size + i] = c_block[j*block_size + i];
               }
            }
         }
      }
      
      delete[] a_block;
      delete[] b_block;
      delete[] c_block;
#ifdef LIKWID_PERFMON
      LIKWID_MARKER_STOP(MY_MARKER_REGION_NAME);
#endif
   }
}

/*
 * Used to warmup the unit, so we measure typical timings, not including
 * one time initialization.
 */
void square_dgemm_blocked_warmup(int n, int block_size, double* A, double* B, double* C)
{
   #pragma omp parallel
   {
      double* a_block = new double[block_size*block_size];
      double* b_block = new double[block_size*block_size];
      double* c_block = new double[block_size*block_size];
      
      int n_blocks = n / block_size;
      #pragma omp for
      for (int block_i = 0; block_i < n_blocks; ++ block_i)
      {
         for (int block_j = 0; block_j < n_blocks; ++block_j)
         {
            // copy the current block of C into c_block
            for (int j = 0; j < block_size; ++j)
            {
               for (int i = 0; i < block_size; ++i)
               {
                  c_block[j*block_size + i] = C[(block_j*block_size + j)*n + block_i*block_size + i];
               }
            }
            
            for (int block_k = 0; block_k < n_blocks; ++block_k)
            {
               // copy the current block of A and B into a_block and b_block
               for (int j = 0; j < block_size; ++j)
               {
                  for (int i = 0; i < block_size; ++i)
                  {
                     a_block[j*block_size + i] = A[(block_k*block_size + j)*n + block_i*block_size + i];
                     b_block[j*block_size + i] = B[(block_j*block_size + j)*n + block_k*block_size + i];
                  }
               }
               
               for (int j = 0; j < block_size; ++j)
               {
                  for (int k = 0; k < block_size; ++k)
                  {
                     for (int i = 0; i < block_size; ++i)
                     {
                        c_block[j*block_size + i] +=
                           a_block[k*block_size + i]*b_block[j*block_size + k];
                     }
                  }
               }
            }
            
            // copy c_block back to the current block of C
            for (int j = 0; j < block_size; ++j)
            {
               for (int i = 0; i < block_size; ++i)
               {
                  C[(block_j*block_size + j)*n + block_i*block_size + i] = c_block[j*block_size + i];
               }
            }
         }
      }
      
      delete[] a_block;
      delete[] b_block;
      delete[] c_block;
   }
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <time.h>

#include "algo_ref.h"

extern double mat[N][N+1];
extern double X[N];



/****** You will replace this routine with your own parallel version *******/
/* Provided global variables are MAXN, N, mat[][], B[], and X[],
 * defined in the beginning of this code.  X[] is initialized to zeros.
 */

int NumThreads;
 struct thread_param{
   int norm;
   int i;
 };

 void *inner_loop(void * param){
     //int norm = *((int *) param);
     struct thread_param* tparam = (struct thread_param*) param;
     int norm = tparam->norm;
     int i = tparam->i;
     //printf("thread = %d\n", norm);
     float multiplier;
     int row, col;
     for (row = norm + i + 1; row < N; row = row + NumThreads) {
         multiplier = mat[row][norm] / mat[norm][norm];
         for (col = norm; col <= N; col++) {
             mat[row][col] -= mat[norm][col] * multiplier;
         }
         //B[row] -= B[norm] * multiplier;
     }
     pthread_exit(0);
 }

void gaussian_parllel(int num_threads) {
  int norm, row, col;  /* Normalization row, and zeroing
			* element row and col */
  float multiplier;
  NumThreads = num_threads;
  pthread_t thread[N];
  int i;

  printf("Computing Serially.\n");

  /* Gaussian elimination */
  for (norm = 0; norm < N - 1; norm++) {
      struct thread_param* param = malloc(num_threads * sizeof(struct thread_param));
      if ( param == NULL ) {
          fprintf(stderr, "Couldn't allocate memory for thread.\n");
          exit(EXIT_FAILURE);
      }

      int i, j;

      for(i = 0; i < num_threads; i++){
        param[i].norm = norm;
        param[i].i = i;
        pthread_create(&thread[i], NULL, inner_loop, (void*) &param[i]);
      }

      for (j = 0; j < num_threads; j++) {
          pthread_join(thread[j], NULL);
      }

      free(param);
  }

  /* (Diagonal elements are not normalized to 1.  This is treated in back
   * substitution.)
   */
  
  backSubs(mat);

}
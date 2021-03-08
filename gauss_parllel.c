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

int NumThreads;
 struct thread_param{
   int norm;
   int id;
 };

 void *inner_loop(void * param){
     //int norm = *((int *) param);
     struct thread_param* tparam = (struct thread_param*) param;
     int norm = tparam->norm;
     int id = tparam->id;
     //printf("thread %d normalizing %d column \n", id, norm);
     float multiplier;
     int row, col;
     for (row = norm + id + 1; row < N; row = row + NumThreads) {
     //printf("   thread %d row= %d\n", id, row);
         multiplier = mat[row][norm] / mat[norm][norm];
         for (col = norm; col <= N; col++) {
             mat[row][col] -= mat[norm][col] * multiplier;
         }
         //printSingle(mat[row]);
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
  printFull(mat, "Initial Equation Matrix");
  
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
        param[i].id = i;
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
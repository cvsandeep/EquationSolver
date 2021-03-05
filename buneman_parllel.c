#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "algo_ref.h"

extern double A[N][N];
extern double X[N];

#define maximum(x, y) (((x) > (y)) ? (x) : (y))
#define minimum(x, y) (((x) < (y)) ? (x) : (y))

pthread_barrier_t barrier;
pthread_mutex_t lock;

int cut;
double Lower[N][N]= {0}, Upper[N][N]={0};
double X[N]= {0},Y[N]= {0};


static void *inner_loop(void *arg){
  int i, j, k;

  int id = *((int *)arg);
  int start = id * cut;
  int end = minimum(start + cut, N);
  //printf("\n Thread:%d, start:%d , end:%d\n", id, start, end);
  for (k = 0; k < N; k++){
    int goof = maximum(k + 1, start);
    for (i = goof; i < end; i++){
      A[i][k] /= A[k][k];
    }
    pthread_barrier_wait(&barrier);
    for(i = goof; i < end; i++){
      for (j = k + 1; j < N; j++){
        A[i][j] -= A[i][k] * A[k][j];
      }
    }
    pthread_barrier_wait(&barrier);
  }

  return NULL;
}

void buneman_parllel(int num_threads){

  pthread_barrier_init(&barrier, NULL, num_threads);

  pthread_t thread[num_threads];

  int arg[num_threads];
  int i,j;
  cut = N / num_threads;
  printf("N=%d , cut=%d\n", N, cut);
  for (i = 0; i < num_threads; i++) {
    arg[i] = i;
    
    pthread_create(&thread[i], NULL, inner_loop, &arg[i]);
  }

  for (i = 0; i < num_threads; i++) {
    pthread_join(thread[i], NULL);
  }
  
  printf("[mat]: \n");
  printMat(A);
  printf("[U]: \n");
  printMat(Upper);
  printf("\n [L]: \n");
  printMat(Lower);
}
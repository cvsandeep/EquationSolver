#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "algo_ref.h"

extern double A[N][N];
extern double X[N];
double B[N];

#define maximum(x, y) (((x) > (y)) ? (x) : (y))
#define minimum(x, y) (((x) < (y)) ? (x) : (y))

pthread_barrier_t barrier;
pthread_mutex_t lock;

int cut;
int Count;
int Remainder;
extern double Lower[N][N], Upper[N][N];
extern double X[N],Y[N];


static void *inner_loop(void *arg){
  int i, norm, k, rows,offset;
  int end;

  int id = *((int *)arg);
  int start = id * cut;
   if (id <= Remainder)
     start += id;
   else
     start += Remainder;
   rows = (id < Remainder ) ? cut +1 : cut ;
   end=start+rows-1;
   if ((offset+rows)==N) 
     end--;
    
  //printf("\n Thread:%d, start:%d , end:%d rows:%d\n", id, start, end, rows);
  for (norm = 0; norm < N; norm++){
    //printf( "\nThread %d, Normalizing %d column:" , id, norm);
    pthread_barrier_wait(&barrier);
    int goof = maximum(norm , start);
    for (i = goof; i <= end; i++){
        //printf("\nLower:%d:[%d][%d] ",id, i,norm);
        if(norm ==0)
          Lower[i][norm] = A[i][norm] / A[norm][norm];
        else
          Lower[i][norm] = Upper[i][norm] / Upper[norm][norm];
    }
    
    pthread_barrier_wait(&barrier);
    
    for (i = goof; i <= end; i++) {
      //printf("\nUpper:%d:", id);
      for (k = norm; k < N; k++){
        if(norm ==0 && i == 0) {
          //printf("************once********\n");
          Upper[i][k]= A[i][k];
          Upper[0][0]= A[0][0];
        }
        else if(i!=0 && norm ==0) {
          //printf("o[%d][%d] ",i,k);  
          Upper[i][k]= A[i][k] - Lower[i][norm]*A[norm][k];
        } else if(i!=norm) {
          //printf("[%d][%d] ",i,k);  
          Upper[i][k]= Upper[i][k] - Lower[i][norm]*Upper[norm][k];
        }
      }
    }
    if(i==norm)
      ;//Lower[i][norm]=1;
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
  Remainder = (N) % num_threads;
  
  //printf("N=%d , cut=%d Remainder=%d\n", N, cut, Remainder );
  for (i = 0; i < num_threads; i++) {
    arg[i] = i;
    
    pthread_create(&thread[i], NULL, inner_loop, &arg[i]);
  }

  for (i = 0; i < num_threads; i++) {
    pthread_join(thread[i], NULL);
  }
  
  
  printMat(A, "Initial Matrix");
  printMat(Upper,"Upper");
  printMat(Lower,"Lower");
  
  for(i=0; i<N; i++)
    {
        Y[i]=B[i];
        for(j=0; j<i; j++)
        {
            Y[i]-=Lower[i][j]*Y[j];
        }
    }
    
    printSingle(Y, "Y with Lower");

    for(i=N-1; i>=0; i--)
    {
        X[i]= Y[i];
        for(j=i+1; j<N; j++)
        {
            X[i]-=Upper[i][j]*X[j];
        }
        X[i]/=Upper[i][i];
    }
    
    printSingle(X, "Solution with Buneman");
}
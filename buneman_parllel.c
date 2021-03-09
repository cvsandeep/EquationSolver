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
int NumThreads;
 struct thread_param{
   int norm;
   int id;
 };

static void *ForwardSub(void *param){
    int i;
    struct thread_param* tparam = (struct thread_param*) param;
     int j = tparam->norm;
     int id = tparam->id;
     //printf("thread %d normalizing %d column \n", id, j);
     for (i = j + id; i < N; i = i + NumThreads) {
       if(j == 0)
          Y[i]=B[i];
       else {
           //printf("-Lower[%d][%d]*y[%d]\n",i,j-1,j-1);
           Y[i]-=Lower[i][j-1]*Y[j-1];
       }
     }
  pthread_exit(0);
}

static void *BackwardSub(void *param){
    int i;
    struct thread_param* tparam = (struct thread_param*) param;
     int j = tparam->norm;
     int id = tparam->id;
     //printf("\nthread %d normalizing %d column \n", id, j);
     for (i = j - id; i >= 0; i = i - NumThreads) {
       if(j == N-1) {
          X[i]=Y[i];
       } else {
           //printf("-Upper[%d][%d]*X[%d]\n",i,j+1,j+1);
           X[i]-=Upper[i][j+1]*X[j+1];
       }
     }
  pthread_exit(0);
}


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
  int i,j,norm;
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
  
  #if 0 
  for(i=0; i<N; i++)
    {
        Y[i]=B[i];
        
        for(j=0; j<i; j++)
        {
            printf("-Lower[%d][%d]*y[%d]",i,j,j);
            Y[i]-=Lower[i][j]*Y[j];
        }
    }
#endif
    
    
    for (norm = 0; norm < N; norm++) {
      struct thread_param* param = malloc(num_threads * sizeof(struct thread_param));
      if ( param == NULL ) {
          fprintf(stderr, "Couldn't allocate memory for thread.\n");
          exit(EXIT_FAILURE);
      }

      for(i = 0; i < num_threads; i++){
        param[i].norm = norm;
        param[i].id = i;
        pthread_create(&thread[i], NULL, ForwardSub, (void*) &param[i]);
      }

      for (j = 0; j < num_threads; j++) {
          pthread_join(thread[j], NULL);
      }

      free(param);
    }
    printSingle(Y, "Y with Lower");

    for (norm = N-1; norm >=0; norm--) {
      struct thread_param* param = malloc(num_threads * sizeof(struct thread_param));
      if ( param == NULL ) {
          fprintf(stderr, "Couldn't allocate memory for thread.\n");
          exit(EXIT_FAILURE);
      }
      

      for(i = 0; i < num_threads; i++){
        param[i].norm = norm;
        param[i].id = i;
        pthread_create(&thread[i], NULL, BackwardSub, (void*) &param[i]);
      }

      for (j = 0; j < num_threads; j++) {
          pthread_join(thread[j], NULL);
      }
      
      if(norm < N)
        X[norm]/=Upper[norm][norm];
      
      free(param);
    }
      
    printSingle(X, "Solution with Buneman");
}
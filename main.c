#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "algo_ref.h"
#define   MAX_NUMP    32
#if (N==8)
double mat[N][N+1] = {{86.3, 41.8, -80.8, -94.3, -10.3, 73.0, 19.0, -89.6, 50.2}, 
                      {71.8, -16.2, 89.9, -91.3, -70.4, -30.2, 26.7, 83.8, 45.3}, 
                      {24.5, 41.7, 43.8, -94.3, -12.7, -73.4, -39.1, -31.4, 24.9},
                      {69.9, 96.5, 22.8, -50.7, -16.8, 80.2, 53.7, -30.5, 48.4},
                      {22, 73, -24.7, -88.3, 45.9, 94.3, -77.9, 17.7, 88.8},
                      {-21.9, -88, 26.4, 7.69, -18.2, -46.9, -8.55, -93.8, 34.2},
                      {94.8, -64.8, -88.1, -18, -38.1, -27.3, 50.7, -68.3, -15.02},
                      {-30.8, -26.5, -19, 52.5, -46.3, -65.3, -78, 75.7, -45.15}};
                      
double A[N][N] = {{86.3, 41.8, -80.8, -94.3, -10.3, 73.0, 19.0, -89.6}, 
                      {71.8, -16.2, 89.9, -91.3, -70.4, -30.2, 26.7, 83.8}, 
                      {24.5, 41.7, 43.8, -94.3, -12.7, -73.4, -39.1, -31.4},
                      {69.9, 96.5, 22.8, -50.7, -16.8, 80.2, 53.7, -30.5},
                      {22, 73, -24.7, -88.3, 45.9, 94.3, -77.9, 17.7},
                      {-21.9, -88, 26.4, 7.69, -18.2, -46.9, -8.55, -93.8},
                      {94.8, -64.8, -88.1, -18, -38.1, -27.3, 50.7, -68.3},
                      {-30.8, -26.5, -19, 52.5, -46.3, -65.3, -78, 75.7}
                       }; 
 double B[N]= {50.2, 45.3, 24.9, 48.4, 88.8, 34.2, -15.02, -45.15};
#elif (N==3)

  double mat[N][N+1] = {{3.0, 2.0,-4.0, 3.0}, 
                       {2.0, 3.0, 3.0, 15.0}, 
						           {5.0,  -3, 1.0, 14.0} }; 


 double A[N][N] = {{2, -1,-2}, 
                  {-4, 6, 3}, 
                  {-4, -2,8} }; 
 double B[N]= {4, -6, 7}; 

#else
double mat[N][N+1];
 double A[N][N];
 double B[N];

#endif

  double X[N]; 

/* Global Definitions*/
 

 int startTime, endTime;
 
 void InitMatrix(void);
 
int main(int argc, char** argv) 
{ 
  unsigned long long int gaussian_serial_time,gaussian_parllel_time, Buneman_serial_time,buneman_parllel_time,runtime;
  struct timespec   StartTime;
  struct timespec   EndTime; 
  int   NumProcs;
  int ret;
  
  srand(time(NULL));
  
  if(argc < 2) 
  {
    fprintf(stderr, "Syntax: %s <numProcesors>\nExiting Program...\n", argv[0]);
    exit(1);
  }

  NumProcs = atoi(argv[1]);

  if (NumProcs < 1 || NumProcs > MAX_NUMP) {
    fprintf(stderr,"Number of processors has to be between 1 and %d\nExiting Program...\n",MAX_NUMP);
    exit(1);
  }

printf("\n ****************gaussian_serial****************\n");
InitMatrix();
ret = clock_gettime(CLOCK_REALTIME, &StartTime); assert(ret == 0);
gaussian_serial(mat);
ret = clock_gettime(CLOCK_REALTIME, &EndTime); assert(ret == 0);
gaussian_serial_time = 1000000000 * (EndTime.tv_sec - StartTime.tv_sec) + EndTime.tv_nsec - StartTime.tv_nsec;

printf("\n ****************gaussian_parllel****************\n");
InitMatrix();
ret = clock_gettime(CLOCK_REALTIME, &StartTime); assert(ret == 0);
gaussian_parllel(NumProcs);
ret = clock_gettime(CLOCK_REALTIME, &EndTime); assert(ret == 0);
gaussian_parllel_time = 1000000000 * (EndTime.tv_sec - StartTime.tv_sec) + EndTime.tv_nsec - StartTime.tv_nsec;

printf("\n ****************Buneman_serial****************\n");
InitMatrix();
ret = clock_gettime(CLOCK_REALTIME, &StartTime); assert(ret == 0);
Buneman_serial(A, B);
ret = clock_gettime(CLOCK_REALTIME, &EndTime); assert(ret == 0);
Buneman_serial_time = 1000000000 * (EndTime.tv_sec - StartTime.tv_sec) + EndTime.tv_nsec - StartTime.tv_nsec;

printf("\n ****************buneman_parllel****************\n");
InitMatrix();
ret = clock_gettime(CLOCK_REALTIME, &StartTime); assert(ret == 0);
buneman_parllel(NumProcs);
ret = clock_gettime(CLOCK_REALTIME, &EndTime); assert(ret == 0);
buneman_parllel_time = 1000000000 * (EndTime.tv_sec - StartTime.tv_sec) + EndTime.tv_nsec - StartTime.tv_nsec;

printf("\ngaussian_serial_time=(%d.%09lld sec)\n", gaussian_serial_time / 1000000000, gaussian_serial_time % 1000000000);
printf("\ngaussian_parllel_time=(%d.%09lld sec)\n", gaussian_parllel_time / 1000000000, gaussian_parllel_time % 1000000000);
printf("\nBuneman_serial_time=(%d.%09lld sec)\n", Buneman_serial_time / 1000000000, Buneman_serial_time % 1000000000);
printf("\nbuneman_parllel_time=(%d.%09lld sec)\n", buneman_parllel_time / 1000000000, buneman_parllel_time % 1000000000);

return 0;
} 

void InitMatrix(void) {
  // Intialize All Matrix  
#if (N>8)
  int i,j;
  
  for (i=0;i<N;i++)
   B[i] =rand() % 100;
  
  for (i=0;i<N;i++)
      for (j=0;j<=N;j++)
       mat[i][j] =rand() % 100;
 
  
  for (i=0;i<N;i++)
      for (j=0;j<N;j++)
      A[i][j] =rand() % 100;
#endif
}

void printFull(double mat[N][N+1]) 
{ 
  int i,j;
	for (i=0; i<N; i++, printf("\n")) 
		for (j=0; j<=N; j++) 
			printf("%lf \t ", mat[i][j]); 
	printf("\n"); 
} 

void printSingle(double x[N]) {
int i;
for (i=0; i<N; i++) 
		printf("%lf\t", x[i]);
   printf("\n"); 
}

// function to print matrix content at any stage 
void printMat(double mat[N][N]) 
{ 
  int i,j;
	for (i=0; i<N; i++, printf("\n")) 
		for (j=0; j<N; j++) 
			printf("%9.3f ", mat[i][j]); 

} 

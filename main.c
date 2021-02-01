#include "algo_ref.h"

 
int main(void) 
{ 
	/* input matrix */
	double mat[N][N+1] = {{3.0, 2.0,-4.0, 3.0}, 
                       {2.0, 3.0, 3.0, 15.0}, 
						           {5.0,  -3, 1.0, 14.0} }; 

	gaussian_serial(mat);
 
 double A[N][N] = {{3.0, 2.0,-4.0}, 
                  {2.0, 3.0, 3.0}, 
                  {5.0,  -3, 1.0} }; 
 double B[N]= {3.0, 15.0, 14.0}; 
 
 Buneman_serial(A, B);

	return 0; 
} 

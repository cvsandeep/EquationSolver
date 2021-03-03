// program to demonstrate working of Guassian Elimination 
// method 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "algo_ref.h"

// function to get matrix content 
void gaussian_serial(double mat[N][N+1]) 
{ 
	/* reduction into r.e.f. */
	int singular_flag = forwardElimn(mat); 

	/* if matrix is singular */
	if (singular_flag != -1) 
	{ 
		printf("Singular Matrix.\n"); 

		/* if the RHS of equation corresponding to 
		zero row is 0, * system has infinitely 
		many solutions, else inconsistent*/
		if (mat[singular_flag][N]) 
			printf("Inconsistent System."); 
		else
			printf("May have infinitely many "
				"solutions."); 

		return; 
	} 

	/* get solution to system and print it using 
	backward substitution */
	backSubs(mat); 
} 

// function for elementary operation of swapping two rows 
void swap_row(double mat[N][N+1], int i, int j) 
{ 
	//printf("Swapped rows %d and %d\n", i, j); 
  int k;
	for (k=0; k<=N; k++) 
	{ 
		double temp = mat[i][k]; 
		mat[i][k] = mat[j][k]; 
		mat[j][k] = temp; 
	} 
} 

// function to print matrix content at any stage 
void print(double mat[N][N+1]) 
{ 
  int i,j;
	for (i=0; i<N; i++, printf("\n")) 
		for (j=0; j<=N; j++) 
			printf("%lf \t ", mat[i][j]); 

	printf("\n"); 
} 

// function to reduce matrix to r.e.f. 
int forwardElimn(double mat[N][N+1]) 
{
  int i,j,k;
	for (k=0; k<N; k++) 
	{ 
		/* if a prinicipal diagonal element is zero, 
		* it denotes that matrix is singular, and 
		* will lead to a division-by-zero later. */
		if (!mat[k][k]) 
			return k; // Matrix is singular 

		for (i=k+1; i<N; i++) 
		{ 
			/* subtract fth multiple of corresponding kth 
			row element*/
			for (j=k+1; j<=N; j++) {
        mat[k][j] =  mat[k][j]/mat[k][k];
				mat[i][j] -= mat[k][j]*mat[i][k];
       } 

			/* filling lower triangular matrix with zeros*/
			mat[i][k] = 0; 
      mat[k][k] = 1;
		} 

		//print(mat);	 //for matrix state 
	} 
	//print(mat);		 //for matrix state 
	return -1; 
} 

// function to calculate the values of the unknowns 
void backSubs(double mat[N][N+1]) 
{ 
	double x[N]; // An array to store solution 
  int i,j;
  
	/* Start calculating from last equation up to the 
	first */
	for (i = N-1; i >= 0; i--) 
	{ 
		/* start with the RHS of the equation */
		x[i] = mat[i][N]; 

		/* Initialize j to i+1 since matrix is upper 
		triangular*/
		for (j=i+1; j<N; j++) 
		{ 
			/* subtract all the lhs values 
			* except the coefficient of the variable 
			* whose value is being calculated */
			x[i] -= mat[i][j]*x[j]; 
		} 

		/* divide the RHS by the coefficient of the 
		unknown being calculated */
		x[i] = x[i]/mat[i][i]; 
	} 

	printf("\nSolution for the system:\n"); 
	for (i=0; i<N; i++) 
		printf("%lf\n", x[i]); 
}

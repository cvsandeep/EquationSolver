#ifndef _ALGO_H 
#define _ALGO_H

#include "matrix.h"

void gaussian_serial(double mat[N][N+1]);
int forwardElimn(double mat[N][N+1]); 
void backSubs(double mat[N][N+1]); 
void gaussian_parllel(int num_threads);

void Buneman_serial(double mat[N][N], double B[N]);

#endif

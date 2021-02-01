#include "matrix.h"

void gaussian_serial(double mat[N][N+1]);
int forwardElimn(double mat[N][N+1]); 
void backSubs(double mat[N][N+1]); 


void Buneman_serial(double mat[N][N], double B[N]);
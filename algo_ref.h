#ifndef _ALGO_H 
#define _ALGO_H

#define N 1000 // Size of Matrix

#define PRINT 0

void gaussian_serial();
void gaussian_parllel(int num_threads);
int forwardElimn(); 
void backSubs(); 


void Buneman_serial();

void printMat(double mat[N][N], char *msg);
void printFull(double mat[N][N+1], char *msg);
void printSingle(double x[N], char *msg);

#endif
#ifndef _ALGO_H 
#define _ALGO_H

#include "matrix.h"

void gaussian_serial();
void gaussian_parllel(int num_threads);
int forwardElimn(); 
void backSubs(); 


void Buneman_serial();

void printMat(double mat[N][N]);
void printFull(double mat[N][N+1]);
void printSingle(double x[N]);

#endif
#include "matrix.h"
#include "stdio.h"

extern double Lower[N][N], Upper[N][N];
extern double X[N],Y[N];

void Buneman_serial(double mat[N][N], double B[N])
{
    int i,j,k;
    
    printMat(mat, "Initial Matrix");
    
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
        {
            if(i<=j)
            {
                //printf("Upper:i[%d]j[%d]",i,j);
                Upper[i][j]=mat[i][j];
                for(k=0; k<=i-1; k++) {
                  //printf(",%d",k);
                    Upper[i][j]-=Lower[i][k]*Upper[k][j];
                }
                if(i==j)
                    Lower[i][j]=1;
                else
                    Lower[i][j]=0;
            }
            else
            {
                //printf("Lower:i[%d]j[%d]",i,j);
                Lower[i][j]=mat[i][j];
                for(k=0; k<=j-1; k++) {
                   // printf(",%d",k);
                    Lower[i][j]-=Lower[i][k]*Upper[k][j];
                }
                Lower[i][j]/=Upper[j][j];
                Upper[i][j]=0;
            }
            
        }
        #if 0
          printMat(Upper, "Upper");
          printMat(Lower, "Lower");
        #endif
        
    }
    
    printMat(Lower,"Lower");
    printMat(Upper, "Upper");
    
    for(i=0; i<N; i++)
    {
        Y[i]=B[i];
        for(j=0; j<i; j++)
        {
            Y[i]-=Lower[i][j]*Y[j];
        }
    }
    
    printSingle(Y, "Y With Lower");

    for(i=N-1; i>=0; i--)
    {
        X[i]= Y[i];
        for(j=i+1; j<N; j++)
        {
            X[i]-=Upper[i][j]*X[j];
        }
        X[i]/=Upper[i][i];
    }
    
    printSingle(X, "LU Solution");
}
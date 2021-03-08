#include "matrix.h"
#include "stdio.h"

void Buneman_serial(double mat[N][N], double B[N])
{
    double Lower[N][N]= {0}, Upper[N][N]={0};
    double X[N]= {0},Y[N]= {0};
    int i,j,k;
    
    printf("[mat]: \n");
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
            printf("%9.3f",mat[i][j]);
        printf("\n");
    }
    
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
        printf("[U]: \n");
        printMat(Upper);
        printf("\n [L]: \n");
        printMat(Lower);
        #endif
        
    }
    printf("[L]: \n");
    printMat(Lower);
    
    printf("\n\n[U]: \n");
    printMat(Upper);
    
    for(i=0; i<N; i++)
    {
        Y[i]=B[i];
        for(j=0; j<i; j++)
        {
            Y[i]-=Lower[i][j]*Y[j];
        }
    }
    printf("\n[Y]: \n");
    printSingle(Y);

    for(i=N-1; i>=0; i--)
    {
        X[i]= Y[i];
        for(j=i+1; j<N; j++)
        {
            X[i]-=Upper[i][j]*X[j];
        }
        X[i]/=Upper[i][i];
    }
    
    printf("\n[X]: \n");
    printSingle(X);
}
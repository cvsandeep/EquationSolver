#include "matrix.h"
#include "stdio.h"

void Buneman_serial(double mat[N][N], double B[N])
{
    double Lower[N][N]= {0}, Upper[N][N];
    double X[N]= {0},Y[N]= {0};
    int i,j,k;
    
    for(j=0; j<N; j++)
    {
        for(i=0; i<N; i++)
        {
            if(i<=j)
            {
                Upper[i][j]=mat[i][j];
                for(k=0; k<i-1; k++)
                    Upper[i][j]-=Lower[i][k]*Upper[k][j];
                if(i==j)
                    Lower[i][j]=1;
                else
                    Lower[i][j]=0;
            }
            else
            {
                Lower[i][j]=mat[i][j];
                for(k=0; k<=j-1; k++)
                    Lower[i][j]-=Lower[i][k]*Upper[k][j];
                Lower[i][j]/=Upper[j][j];
                Upper[i][j]=0;
            }
        }
    }
    printf("[L]: \n");
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
            printf("%9.3f",Lower[i][j]);
        printf("\n");
    }
    printf("\n\n[U]: \n");
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
            printf("%9.3f",Upper[i][j]);
        printf("\n");
    }
    for(i=0; i<N; i++)
    {
        Y[i]=B[i];
        for(j=0; j<i; j++)
        {
            Y[i]-=Lower[i][j]*Y[j];
        }
    }
    printf("\n\n[Y]: \n");
    for(i=0; i<N; i++)
    {
        printf("%9.3f",Y[i]);
    }
    for(i=N-1; i>=0; i--)
    {
        X[i]= Y[i];
        for(j=i+1; j<N; j++)
        {
            X[i]-=Upper[i][j]*X[j];
        }
        X[i]/=Upper[i][i];
    }
    printf("\n\n[X]: \n");
    for(i=0; i<N; i++)
    {
        printf("%9.3f",X[i]);
    }
     printf("\n\n");
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* k-NN algorithm by Ander Movilla.
Implementation in C of the simple machine learning algorithm k nearest neighbours (k-NN).
Given a set of predictors (3 per measurement) and their result gives predictions for 
future instants of time. The prediction is the average of the results of the k nearest 
points using Euclidean distance. The quadratic error per trimester is given for the moments 
in time where a result is available.  */

/*Variable declaration.*/

int main() {

	FILE *fp;
	int n=6;
	char str1[130], str2[120];
	char str3[n+1][100];
	double  a[2874][6];
	int i, j, l,m, f1, f2, f3 ;
	
	double  b[n][5];
double **c = malloc(sizeof *c * 2874);
if (c)
{
  for (i = 0; i < 2874; i++)
  {
    c[i] = malloc(sizeof *c[i] * 2874);
  }
}
	double  c2[2874][n];
	double  d2;
	int k=20;
	double  sort[2874][k];
	int  dates[n][3];
	double  sort2[n][k];
	double  prod2[n][k];
	double  prod[2874][k];
	double  pred[2874];
	double  pred2[n];
	double  obs[2874];
	double  ecm[5];
	double  emabs[5];
	int  trim[5];
	int  tr;
	double  dist, sum, sum2;

	double * observs;
        double * predics;

/* Reading the data. The a array stores the past values, the array b the future predictions.*/
   fp = fopen("Example_Data.txt", "r");
   fscanf(fp, "%s", str1);
	for ( i =0 ; i < 2874; i++ ) {
		for (j=1; j<6; j++){
			if(j<2) {
				fscanf(fp, "%d-%d-%d",&f1,&f2,&f3);
				fscanf(fp, "%d:%d", &f1,&f3);
				a[i][1]=f2;
				}
			else{
				fscanf(fp, "%lf", &a[i][j]);
			}
		}
   	}
   fscanf(fp, "%s", str2);
	for ( i = 0; i <n ; i++ ) {
		for (j=1; j<5; j++){
			if(j<2) {
				fscanf(fp, "%d-%d-%d",&f1,&f2,&f3);
				dates[i][1]=f1;
				dates[i][2]=f2;
				dates[i][3]=f3;
				fscanf(fp, "%d:%d", &f1,&f3);
				}
			else{
				fscanf(fp, "%lf", &b[i][j]);
			}
      		}
   	}
   fclose(fp);

/* Distance calculation. Stored in c and c2. */
	for ( l = 0; l<2874;l++){	
		for ( i = 0; i < 2874; i++ ) {
			dist=0;
			for (j=1; j<4; j++){
				dist=dist+pow(a[l][j+2]-a[i][j+2],2);
			}
			c[i][l]=sqrt(dist);
      		}
   	}
	for ( l = 0; l<n;l++){	
		for ( i = 0; i < 2874; i++ ) {
			dist=0;
			for (j=1; j<4; j++){
				dist=dist+pow(b[l][j+1]-a[i][j+2],2);
			}
			c2[i][l]=sqrt(dist);
      		}
   	}
/*sort and sort2 initialisation.*/
	for (j=0; j<2874; j++){
			for (l=0; l<k; l++){
				sort[j][l]=1e6;
			}
		}
	for (j=0; j<n; j++){
			for (l=0; l<k; l++){
				sort2[j][l]=1e6;
			}
		}
/* Storing the k smaller distances for each case into the sort and sort 2 arrays. */
	for (i=0; i<2874; i++){
		for (j=0; j<2874; j++){
			for (l=0; l<k; l++){
				if(sort[i][l] > c[j][i]){
					for (m=l; m<(k-1); m++){
						sort[i][m+1]=sort[i][m];
						prod[i][m+1]=prod[i][m];
					}
					sort[i][l]=c[j][i];
					prod[i][l]=a[j][2];
					break;
				}
			}
		}
	}
	for (i=0; i<n; i++){
		for (j=0; j<2874; j++){
			for (l=0; l<k; l++){
				if(sort2[i][l]>c2[j][i]){
					for (m=l; m<(k-1); m++){
						sort2[i][m+1]=sort2[i][m];
						prod2[i][m+1]=prod2[i][m];
					}
					sort2[i][l]=c2[j][i];
					prod2[i][l]=a[j][2];
					break;
				}
			}
		}	
	}
/*Computing averages for the predictions.*/
	for (i=0; i<2874; i++){
		sum=0;
		for (l=0; l<k; l++){
			sum=sum+prod[i][l];
		}
		pred[i]=sum/k;
	}

	for (i=0; i<n; i++){
		sum=0;
		for (l=0; l<k; l++){
			sum=sum+prod2[i][l];
		}
		pred2[i]=sum/k;
	}
/*Searching for the end of the trimester. */
trim[0]=0;
	for (i=1; i<=4; i++){
		for (j=0; j<2874; j++){
			tr=ceil(a[j][1]/3);
			if (i == tr){
				trim[i]=j;
			}
		}
	}
/*Cálculo pdelerror cuadrático medio por trimestre.*/
	for (i=1; i<=4; i++){
		sum=0;
		sum2=0;
		for (j=0; j<2874; j++){
			if ( i==ceil(a[j][1]/3)){
				sum += a[j][2];
				sum2 += pow(a[j][2]-pred[j],2);
			}
		}
		ecm[i]=100*(trim[i]-trim[i-1])*sqrt(sum2/(trim[i]-trim[i-1]))/sum;
	}

/* Data output to file preditions.txt. */
   fp = fopen("predictions.txt", "w+");
	for (i=1; i<=4; i++){

   fprintf(fp, "Trimester %d: %.2f, %.2f \n", i,  ecm[i]);
	}
	for (i=0; i<n; i++){
   fprintf(fp, "%d-%d-%d: %.0f \n", dates[i][1],dates[i][2], dates[i][3], pred2[i]);
	}
   fclose(fp);

  return 0;


}

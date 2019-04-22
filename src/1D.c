/*

//This is for 1D case for one measurement (the position)
//THOROUGHLY TESTED BUT COULD BE FAULTY

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ELEMENTS 10
#define TIME 1

int main()
{
	float xLocErr=20;
	float xLoc=4000, xVel=0;
	float xLocMea;
	float acc=(-9.8);
	float x[2][1], p[2][2], P[2][2];
	float upperK[2][1], lowerK1[1][2], lowerK2, inverseLowerK, K[2][1];
	float y; //for updating the state, used in step 4
	float predicted[ELEMENTS], updated[ELEMENTS];

	float prevP[2][2] = {
			{0,0},
			{0,0}
	};

	//need to be tuned
	float Q[2][2] = {
			{4.76,0},
			{0,30}
	};

	float prevX[2][1] = {
			{xLoc},
			{xVel}
	};

	float A[2][2] = {
			{1,TIME},
			{0,1}
	};

	float At[2][2] = {
			{1,0},
			{TIME,1}
	};

	float B[2][1] = {
			{(TIME*TIME)*0.5},
			{TIME}
	};

	float C[1][2] = {
			{1,0}
	};

	float Ct[2][1] = {
			{1},
			{0}
	};

	float R = xLocErr*xLocErr;

	float I[2][2] = {
			{1,0},
			{0,1}
	};

	FILE *fp;
	fp = fopen("xLocation.txt","r");

	int count = 0; //used at line 161 and 162 for the arrays

	while(!feof(fp)) {

		//printf("%d. ", count);
	    //printf("At t = %ds:\n", count);
		fscanf(fp, "%f", &xLocMea);

		float z = xLocMea;


		//1.Predict state
		x[0][0] = (A[0][0]*prevX[0][0])+(A[0][1]*prevX[1][0])+(B[0][0]*acc);
		x[1][0] = (A[1][0]*prevX[0][0])+(A[1][1]*prevX[1][0])+(B[1][0]*acc);

		//2.Calculate process covariance matrix
		for(int i=0;i<=1;i++) {
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=1;k++) {
					sum = sum + A[i][k]*prevP[k][j];
				}
				p[i][j] = sum;
			}
		}

		for(int i=0;i<=1;i++) {
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=1;k++) {
					sum = sum + p[i][k]*At[k][j];
				}
				p[i][j] = sum + Q[i][j];
			}
		}

		//3.Calculate Kalman Gain
		for(int i=0;i<=1;i++) {
			for(int j=0;j<=0;j++) {
				float sum = 0;
				for(int k=0;k<=1;k++) {
					sum = sum + p[i][k]*Ct[k][j];
				}
				upperK[i][j] = sum;
			}
		}

		//lower K is separated into two parts due to different matrix form
		for(int i=0;i<=0;i++) {
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=1;k++) {
					sum = sum + C[i][k]*p[k][j];
				}
				lowerK1[i][j] = sum;
			}
		}

		lowerK2 = (lowerK1[0][0]*Ct[0][0])+(lowerK1[0][1]*Ct[1][0])+R;
		inverseLowerK = 1/lowerK2;

		K[0][0] = upperK[0][0]*inverseLowerK;
		K[1][0] = upperK[1][0]*inverseLowerK;

		//4.Update state
		y = z-((C[0][0]*x[0][0])+(C[0][1]*x[1][0]));

		prevX[0][0] = x[0][0]+(K[0][0]*y);
	    prevX[1][0] = x[1][0]+(K[1][0]*y);

		//5. Update process covariance matrix
		for(int i=0;i<=1;i++) {
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=0;k++) {
					sum = sum + K[i][k]*C[k][j];
				}
				P[i][j] = I[i][j]-sum;
			}
		}

		for(int i=0;i<=1;i++) {
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=1;k++) {
					sum = sum + P[i][k]*p[k][j];
				}
				prevP[i][j] = sum;
			}
		}

		predicted[count] = x[0][0];
		updated[count] = prevX[0][0];

		//the following printf is optional
		printf("CURRENT STATE:\n");
		printf("%f\n",prevX[0][0]);
		printf("%f\n",prevX[1][0]);
		printf("\n");

		printf("PROCESS COVARIANCE MATRIX:\n");
		for(int i=0;i<=1;i++) {
			for(int j=0;j<=1;j++) {
				printf("%f, ",prevP[i][j]);
			}
			printf("\n");
		}
		printf("\n");


		count++;
	}

	//to print out the predicted and updated position onto console for easier plotting in excel
	for(int i=0;i<count;i++) {
		printf("%.2f\n", predicted[i]);
	}

	printf("\n");

	for(int i=0;i<count;i++) {
		printf("%.2f\n", updated[i]);
	}

	fclose(fp);
}
*/

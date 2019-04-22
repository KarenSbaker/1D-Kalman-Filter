/*

//This is 2D case with xLocation and yLocation measurement
//WARNING: NOT TESTED YET

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define TIME 1

int main()
{
	float xLocErr=20, yLocErr=20; //for the sensor error
	float xLoc=4000, yLoc=4000, xVel=0, yVel=0;
	float xLocMea, yLocMea;
	float acc=(-9.8);
	float x[4][1], p[4][4], P[4][4];
	float upperK[4][2], lowerK1[2][4], lowerK2[2][2], inverseLowerK[2][2], K[4][2];
	float a,b,c,d,e; //for calculating inverseLowerK
	float y[2][1]; //for updating the state, used in step 4

	float prevP[4][4] = {
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0}
	};

	float prevX[4][1] = {
			{xLoc},
			{yLoc},
			{xVel},
			{yVel}
	};

	float A[4][4] = {
			{1,0,TIME,0},
			{0,1,0,TIME},
			{0,0,1,0},
			{0,0,0,1}
	};

	float At[4][4] = {
			{1,0,0,0},
			{0,1,0,0},
			{TIME,0,1,0},
			{0,TIME,0,1}
	};

	float B[4][1] = {
			{(TIME*TIME)*0.5},
			{(TIME*TIME)*0.5},
			{TIME},
			{TIME}
	};

	float C[2][4] = {
			{1,0,0,0},
			{0,1,0,0}
	};

	float Ct[4][2] = {
			{1,0},
			{0,1},
			{0,0},
			{0,0}
	};

	//need to be tuned
	float Q[4][4] = {
			{9,0,0,0},
			{0,9,0,0},
			{0,0,4,0},
			{0,0,0,4}
	};

	//constant
	float R[2][2] = {
			{xLocErr*xLocErr,0},
			{0,yLocErr*yLocErr}
	};

	float I[4][4] = {
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,1}
	};

	//open file for reading measurements
	FILE *fp1, *fp2;
	fp1 = fopen("xLocation.txt","r");
	fp2 = fopen("yLocation.txt","r");

	while(!feof(fp1)) {

		fscanf(fp1, "%f", &xLocMea);
		fscanf(fp2, "%f", &yLocMea);

		float z[2][1] = {
				{xLocMea},
				{yLocMea}
		};

		//1.Predict state
		x[0][0] = (A[0][0]*prevX[0][0])+(A[0][1]*prevX[1][0])+(A[0][2]*prevX[2][0])+(A[0][3]*prevX[3][0])+(B[0][0]*acc);
		x[1][0] = (A[1][0]*prevX[0][0])+(A[1][1]*prevX[1][0])+(A[1][2]*prevX[2][0])+(A[1][3]*prevX[3][0])+(B[1][0]*acc);
		x[2][0] = (A[2][0]*prevX[0][0])+(A[2][1]*prevX[1][0])+(A[2][2]*prevX[2][0])+(A[2][3]*prevX[3][0])+(B[2][0]*acc);
		x[3][0] = (A[3][0]*prevX[0][0])+(A[3][1]*prevX[1][0])+(A[3][2]*prevX[2][0])+(A[3][3]*prevX[3][0])+(B[3][0]*acc);

		//2.Predict process covariance matrix
		for(int i=0;i<=3;i++) {
			for(int j=0;j<=3;j++) {
				float sum = 0;
				for(int k=0;k<=3;k++) {
					sum = sum + A[i][k]*prevP[k][j];
				}
				p[i][j] = sum;
			}
		}

		for(int i=0;i<=3;i++) {
			for(int j=0;j<=3;j++) {
				float sum = 0;
				for(int k=0;k<=3;k++) {
					sum = sum + p[i][k]*At[k][j];
				}
				p[i][j] = sum + Q[i][j];
			}
		}

		//3.Calculate Kalman Gain
		for(int i=0;i<=3;i++) {
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=3;k++) {
					sum = sum + p[i][k]*Ct[k][j];
				}
				upperK[i][j] = sum;
			}
		}

		//lower K is separated into two parts due to the different matrix form
		for(int i=0;i<=1;i++) {
			for(int j=0;j<=3;j++) {
				float sum = 0;
				for(int k=0;k<=3;k++) {
					sum = sum + C[i][k]*p[k][j];
				}
				lowerK1[i][j] = sum;
			}
		}

		for(int i=0;i<=1;i++) {
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=3;k++) {
					sum = sum + lowerK1[i][k]*Ct[k][j];
				}
				lowerK2[i][j] = sum + R[i][j];
			}
		}

		e = 1/((lowerK2[0][0]*lowerK2[1][1])-(lowerK2[0][1]*lowerK2[1][0]));
		a = lowerK2[0][0];
		b = lowerK2[0][1];
		c = lowerK2[1][0];
		d = lowerK2[1][1];

		inverseLowerK[0][0] = e*d;
		inverseLowerK[0][1] = e*(-b);
		inverseLowerK[1][0] = e*(-c);
		inverseLowerK[1][1] = e*a;

		for(int i=0;i<=3;i++) {
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=1;k++) {
					sum = sum + upperK[i][k]*inverseLowerK[k][j];
				}
				K[i][j] = sum;
			}
		}

		//4.Update state
		y[0][0] = z[0][0]-((C[0][0]*x[0][0])+(C[0][1]*x[1][0])+(C[0][2]*x[2][0])+(C[0][3]*x[3][0]));
		y[1][0] = z[1][0]-((C[1][0]*x[0][0])+(C[1][1]*x[1][0])+(C[1][2]*x[2][0])+(C[1][3]*x[3][0]));

		prevX[0][0] = x[0][0]+((K[0][0]*y[0][0])+(K[0][1]*y[1][0]));
	    prevX[1][0] = x[1][0]+((K[1][0]*y[0][0])+(K[1][1]*y[1][0]));
		prevX[2][0] = x[2][0]+((K[2][0]*y[0][0])+(K[2][1]*y[1][0]));
		prevX[3][0] = x[3][0]+((K[3][0]*y[0][0])+(K[3][1]*y[1][0]));

		//5. Update process covariance matrix
		for(int i=0;i<=3;i++) {
			for(int j=0;j<=3;j++) {
				float sum = 0;
				for(int k=0;k<=1;k++) {
					sum = sum + K[i][k]*C[k][j];
				}
				P[i][j] = I[i][j]- sum;
			}
		}

		for(int i=0;i<=3;i++) {
			for(int j=0;j<=3;j++) {
				float sum = 0;
				for(int k=0;k<=3;k++) {
					sum = sum + P[i][k]*p[k][j];
				}
				prevP[i][j] = sum;
			}
		}

		//the following printf is optional
		printf("STATE ESTIMATE:\n");
		printf("%f\n",prevX[0][0]);
		printf("%f\n",prevX[1][0]);
		printf("%f\n",prevX[2][0]);
		printf("%f\n",prevX[3][0]);

		printf("PROCESS COVARIANCE MATRIX:\n");
		for(int i=0;i<=3;i++) {
			for(int j=0;j<=3;j++) {
				printf("%f, ",prevP[i][j]);
			}
			printf("\n");
		}
	}
	fclose(fp1);
	fclose(fp2);
}
*/

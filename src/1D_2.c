/*

//This is for 1D case with two measurements (the position and the velocity)
//Different C and R from the other 1D case, calculation step 3-5 is also slightly different
//UNSTABLE, NEED CHECKING

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ELEMENTS 10
#define TIME 1

int main()
{
	float xLocErr=20;
	float xVelErr=2;
	float xLoc=4000, xVel=0;
	float xLocMea, xVelMea;
	float acc=(-9.8);
	float x[2][1], p[2][2], P[2][2];
	float upperK[2][2], lowerK[2][2], K[2][2];
	float y[2][1]; //for updating the state
	float predicted[ELEMENTS], updated[ELEMENTS];

	float prevP[2][2] = {
			{0,0},
			{0,0}
	};

	//constant
	float Q[2][2] = {
			{4.76,0},
			{0,19.02}
	};

	float R[2][2] = {
			{xLocErr*xLocErr,0},
			{0,xVelErr*xVelErr}
	};

	//needs to be fed
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

	float C[2][2] = {
			{1,0},
			{0,1}
	};

	float Ct[2][2] = {
			{1,0},
			{0,1}
	};

	float I[2][2] = {
			{1,0},
			{0,1}
	};

	FILE *fp1, *fp2;
	fp1 = fopen("xLocation.txt","r");
	fp2 = fopen("xVelocity.txt","r");

	int count = 0;

	while(!feof(fp1)) {

		//printf("%d. ", count);
	    //printf("At t = %ds:\n", count);
		fscanf(fp1, "%f", &xLocMea);
		fscanf(fp2, "%f", &xVelMea);

		float z[2][1] = {
				{xLocMea},
				{xVelMea}
		};


		//1.Predict state
		x[0][0] = (A[0][0]*prevX[0][0])+(A[0][1]*prevX[1][0])+(B[0][0]*acc);
		x[1][0] = (A[1][0]*prevX[0][0])+(A[1][1]*prevX[1][0])+(B[1][0]*acc);
		printf("Predicted state:\n");
		printf("%f\n", x[0][0]);
		printf("%f\n", x[1][0]);

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
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=1;k++) {
					sum = sum + p[i][k]*Ct[k][j];
				}
				upperK[i][j] = sum;
			}
		}

		for(int i=0;i<=1;i++) {
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=1;k++) {
					sum = sum + C[i][k]*p[k][j];
				}
				lowerK[i][j] = sum;
			}
		}

		for(int i=0;i<=1;i++) {
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=1;k++) {
					sum = sum + lowerK[i][k]*Ct[k][j];
				}
				lowerK[i][j] = sum + R[i][j];
			}
		}
		//printf("%f\n", upperK[0][1]);
		//printf("%f\n", lowerK[0][1]);

		K[0][0] = upperK[0][0]/lowerK[0][0];
		K[0][1] = 0;
		K[1][0] = 0;
		K[1][1] = upperK[1][1]/lowerK[1][1];

		printf("\nKalman gain:\n");
		printf("%f  %f\n", K[0][0],K[0][1]);
		printf("%f  %f\n", K[1][0],K[1][1]);

		//4.Update state
		y[0][0] = z[0][0]-((C[0][0]*x[0][0])+(C[0][1]*x[1][0]));
		y[1][0] = z[1][0]-((C[1][0]*x[0][0])+(C[1][1]*x[1][0]));

		prevX[0][0] = x[0][0]+((K[0][0]*y[0][0])+(K[0][1]+y[1][0]));
	    prevX[1][0] = x[1][0]+((K[1][0]*y[0][0])+(K[1][1]+y[1][0]));

		//5. Update process covariance matrix
		for(int i=0;i<=1;i++) {
			for(int j=0;j<=1;j++) {
				float sum = 0;
				for(int k=0;k<=1;k++) {
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

		//printf("PREDICTED LOCATION: ");

		//printf("%.2f\n", x[0][0]);
		predicted[count] = x[0][0];

		//printf("LOCATION MEASUREMENT: ");
		//printf("%f\n", xLocMea);
		//printf("UPDATED LOCATION: ");

		//printf("%.2f\n",prevX[0][0]);
		updated[count] = prevX[0][0];

		printf("\nCURRENT STATE:\n");
		printf("%f\n",prevX[0][0]);
		printf("%f\n",prevX[1][0]);

		printf("\nPROCESS COVARIANCE MATRIX:\n");
		for(int i=0;i<=1;i++) {
			for(int j=0;j<=1;j++) {
				printf("%f, ",prevP[i][j]);
			}
			printf("\n");
		}
		printf("\n");

		count++;
	}
	for(int i=0;i<count;i++) {
		printf("%.2f\n", predicted[i]);
	}

	printf("\n");

	for(int i=0;i<count;i++) {
		printf("%.2f\n", updated[i]);
	}

	fclose(fp1);
	fclose(fp2);
}
*/

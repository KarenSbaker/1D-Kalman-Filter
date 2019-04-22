/*

//WITH AIR RESISTANCE
//An example of a basketball with mass of 0.6kg and a diameter of 9inches/0.2286m falling

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ELEMENTS 10
#define TIME 1

int main()
{
	double V1 = 0;
	double V2 = 0;
	//double V3 = 0;
	double X1 = 4000;
	double X2 = 4000;
	double a = 9.8; //will update with resistance
	double g = 9.8; //constant
	double m = 0.6; //mass of ball
	double r = 1.225;
	double A = 0.041; //cross-sectional area of ball (pi*radius^2)
	double Cd = 0.47; //coefficient of drag
	double positionWithoutResistance[ELEMENTS];
	double positionWithResistance[ELEMENTS];
	double velocity[ELEMENTS];
	int random1, random2; //random1 is for adding error to location, random2 is for adding error to velocity

	for(int i=0;i<ELEMENTS;i++){

		V1 = V2;
		V2 = V1 + (a*TIME);
		//V3 = V1 + (g*time);
		X1 = X2 - ((V1*TIME)+(0.5*g*(pow(TIME,2))));
		X2 = X2 - ((V1*TIME)+(0.5*a*(pow(TIME,2))));
		a = (m*g - (0.5*r*A*Cd*(pow(V1,2))))/m;
		if(a<0) {
			a=0;
		}

		//printf("Acceleration is %f\n", a);
	    //printf("V1 is %f\n", V1);
		//printf("V2 is %f\n", V2);
		//printf("V3 is %f\n", V3);
		//printf("X1 is %.2f\n", X1);
		//printf("X2 is %.2f\n", X2);
		positionWithoutResistance[i] = X1;
		positionWithResistance[i] = X2;
		velocity[i] = V1;
	}

	//file fp1 is where all the data will be written to
	//fp2 is for storing the xLocation for 1D program to read from
	//fp3 is for storing the xVelocity for 1D program to read from
	FILE *fp1, *fp2, *fp3;
	fp1 = fopen("Data.txt","w");
	fp2 = fopen("xLocation.txt","w");
	fp3 = fopen("xVelocity.txt","w");

	fprintf(fp1, "Position WITHOUT resistance:\n");
	for(int i=0;i<ELEMENTS;i++) {
		fprintf(fp1, "%.2f\n", positionWithoutResistance[i]);
	}

	fprintf(fp1, "\nPosition WITH resistance:\n");
	for(int i=0;i<ELEMENTS;i++) {
		fprintf(fp1, "%.2f\n", positionWithResistance[i]);
	}

	fprintf(fp1, "\nPosition WITH resistance WITH error:\n");
	for(int i=0;i<ELEMENTS;i++) {
		//a random error of +-20 is added to position as "sensor error"
		for(;;) {
			random1 = ((rand()%3)-1)*20;
			if(random1==0){
				continue;
			}
			else break;
		}
		positionWithResistance[i] = positionWithResistance[i] + random1;
		fprintf(fp1, "%.2f\n", positionWithResistance[i]);
		fprintf(fp2, "%.2f\n", positionWithResistance[i]);
	}

	fprintf(fp1, "\nVelocity WITHOUT error:\n");
	for(int i=0;i<ELEMENTS;i++) {
		fprintf(fp1, "%.2f\n", velocity[i]);
	}

	fprintf(fp1, "\nVelocity WITH error:\n");
	for(int i=0;i<ELEMENTS;i++) {
		//a random error of +-2 is added to velocity as "sensor error"
		for(;;) {
			random2 = ((rand()%3)-1)*2;
			if(random2==0){
				continue;
			}
			else break;
		}
		velocity[i] = velocity[i] + random2;
		fprintf(fp1, "%.2f\n", velocity[i]);
		fprintf(fp3, "%.2f\n", velocity[i]);
	}

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);

}
*/

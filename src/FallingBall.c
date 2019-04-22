/*

//WITHOUT AIR RESISTANCE
//Perfect and unrealistic example of a free falling object

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ELEMENTS 10

int main()
{
	double initD = 4000;
	double d = 0;
	double initV = 0;
	double a = 9.8;
	int random;

	FILE *fp;
	fp = fopen("BallFalling.txt","w");


	for(int t=0; t<ELEMENTS; t++) {
		d = (initV*t) + (0.5*a*(pow(t,2)));
		printf("Position without error:\n"); //print out position at every second without error onto console
		printf("%.2f\n", initD-d);
		for(;;) {
			random = ((rand()%3)-1)*20;
			if(random==0){
				continue;
			}
			else break;
		}
		fprintf(fp, "%.2f\n", (initD-d)+random); //add a random error of +-20 to output file
	}

	fclose(fp);
}
*/

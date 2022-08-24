//
//  Main1.c
//  
//
//  Created by Matteo Federico on 24/08/22.
//
#include <stdio.h>
#include "Main.h"
#include "libreriaRand/rngs.h" //spostare le librerie
#include "librerieProgetto/struct.h"
#include "Simulatore.h"

#define N 64

void media(output matrix[N][12])
{
	double somma1[12];
	double somma2[12];
	double somma3[12];
	double somma4[12];
	double somma5[12];
	double somma6[12];

	for(int j=0;j<12;j++)
	{
		somma1[j]=0.0;
		somma2[j]=0.0;
		somma3[j]=0.0;
		somma4[j]=0.0;
		somma5[j]=0.0;
		somma6[j]=0.0;
		for(int i=0;i<N;i++)
		{
			somma1[j]=somma1[j]+matrix[i][j].wait;
			somma2[j]=somma2[j]+matrix[i][j].delay;
			somma3[j]=somma3[j]+matrix[i][j].service;
			somma4[j]=somma4[j]+matrix[i][j].numberNode;
			somma5[j]=somma5[j]+matrix[i][j].numberQueue;
			somma6[j]=somma6[j]+matrix[i][j].utilization;
		}
		printf("j=%d]-------------------------\n",j);
		printf("1] %f\n",somma1[j]/N);
		printf("2] %f\n",somma2[j]/N);
		printf("3] %f\n",somma3[j]/N);
		printf("4] %f\n",somma4[j]/N);
		printf("5] %f\n",somma5[j]/N);
		printf("6] %f\n",somma6[j]/N);
	}


}




int main(){
	output matrix[N][12];
	PlantSeeds(SEED);
	for(int i=0;i<N;i++){
		simulatore(matrix[i]);	
	}
	media(matrix);
	return 0;
}



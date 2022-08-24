//
//  Main1.c
//  
//
//  Created by Matteo Federico on 24/08/22.
//
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include "Main.h"
#include "libreriaRand/rngs.h" //spostare le librerie
#include "libreriaRand/rvms.h" 
#include "librerieProgetto/struct.h"
#include "Simulatore.h"

#define N 64
#define ALFA 0.04
void azzeraOutput(output matrix[N][12]){
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<12;j++){
			matrix[i][j].wait=0.0;
			matrix[i][j].delay=0.0;
			matrix[i][j].service=0.0;
			matrix[i][j].numberNode=0.0;
			matrix[i][j].numberQueue=0.0;
			matrix[i][j].utilization=0.0;
			matrix[i][j].job=0;
		}
	}

}
void media(output matrix[N][12], double med[7][12])
{
	double somma1[12];
	double somma2[12];
	double somma3[12];
	double somma4[12];
	double somma5[12];
	double somma6[12];
	double somma7[12];

	for(int j=0;j<12;j++)
	{
		somma1[j]=0.0;
		somma2[j]=0.0;
		somma3[j]=0.0;
		somma4[j]=0.0;
		somma5[j]=0.0;
		somma6[j]=0.0;
		somma7[j]=0.0;
		for(int i=0;i<N;i++)
		{
			if(matrix[i][j].wait==matrix[i][j].wait)
				somma1[j]=somma1[j]+matrix[i][j].wait;
			if(matrix[i][j].delay==matrix[i][j].delay)
				somma2[j]=somma2[j]+matrix[i][j].delay;
			if(matrix[i][j].service==matrix[i][j].service)
				somma3[j]=somma3[j]+matrix[i][j].service;
			
			somma4[j]=somma4[j]+matrix[i][j].numberNode;
		
			somma5[j]=somma5[j]+matrix[i][j].numberQueue;
			
			somma6[j]=somma6[j]+matrix[i][j].utilization;

			somma7[j]=somma7[j]+matrix[i][j].job;
		}
		med[0][j]=somma1[j]/N;
		med[1][j]=somma2[j]/N;
		med[2][j]=somma3[j]/N;
		med[3][j]=somma4[j]/N;
		med[4][j]=somma5[j]/N;
		med[5][j]=somma6[j]/N;
		med[6][j]=somma7[j]/N;
		printf("j=%d]-------------------------\n",j);
		printf("1] wait %f\n",somma1[j]/N);
		printf("2] delay %f\n",somma2[j]/N);
		printf("3] service %f\n",somma3[j]/N);
		printf("4] numberNode %f\n",somma4[j]/N);
		printf("5] numberQueue %f\n",somma5[j]/N);
		printf("6] utilization %f\n",somma6[j]/N);
		printf("7] job %f \n", (somma7[j])/N);
	}
}

void varianza(output matrix[N][12],double med[7][12],double var[7][12],double omega[7][12])
{
	double tstar=idfStudent(N-1,1-ALFA/2);
	double somma1[12];
	double somma2[12];
	double somma3[12];
	double somma4[12];
	double somma5[12];
	double somma6[12];
	double somma7[12];
	for(int j=0;j<12;j++)
	{
		somma1[j]=0.0;
		somma2[j]=0.0;
		somma3[j]=0.0;
		somma4[j]=0.0;
		somma5[j]=0.0;
		somma6[j]=0.0;
		somma7[j]=0.0;
		for(int i=0;i<N;i++)
		{
			if(matrix[i][j].wait==matrix[i][j].wait)
				somma1[j]=somma1[j]+ (matrix[i][j].wait-med[0][j])*(matrix[i][j].wait-med[0][j]);
			else
				somma1[j]=somma1[j]+ (med[0][j])*(med[0][j]);

			if(matrix[i][j].delay==matrix[i][j].delay)
				somma2[j]=somma2[j]+(matrix[i][j].delay-med[1][j])*(matrix[i][j].delay-med[1][j]);
			else
				somma2[j]=somma2[j]+(med[1][j])*(med[1][j]);

			if(matrix[i][j].service==matrix[i][j].service)
				somma3[j]=somma3[j]+(matrix[i][j].service-med[2][j])*(matrix[i][j].service-med[2][j]);
			else
				somma3[j]=somma3[j]+(med[2][j])*(med[2][j]);

			somma4[j]=somma4[j]+(matrix[i][j].numberNode-med[3][j])*(matrix[i][j].numberNode-med[3][j]);
		
			somma5[j]=somma5[j]+(matrix[i][j].numberQueue-med[4][j])*(matrix[i][j].numberQueue-med[4][j]);
			
			somma6[j]=somma6[j]+(matrix[i][j].utilization-med[5][j])*(matrix[i][j].utilization-med[5][j]);

			somma7[j]=somma7[j]+(matrix[i][j].job-med[6][j])*(matrix[i][j].job-med[6][j]);
		}
		var[0][j]=somma1[j]/N;
		var[1][j]=somma2[j]/N;
		var[2][j]=somma3[j]/N;
		var[3][j]=somma4[j]/N;
		var[4][j]=somma5[j]/N;
		var[5][j]=somma6[j]/N;
		var[6][j]=somma7[j]/N;	
		omega[0][j]=tstar*sqrt(var[0][j])/sqrt(N-1);
		omega[1][j]=tstar*sqrt(var[1][j])/sqrt(N-1);
		omega[2][j]=tstar*sqrt(var[2][j])/sqrt(N-1);
		omega[3][j]=tstar*sqrt(var[3][j])/sqrt(N-1);
		omega[4][j]=tstar*sqrt(var[4][j])/sqrt(N-1);
		omega[5][j]=tstar*sqrt(var[5][j])/sqrt(N-1);
		omega[6][j]=tstar*sqrt(var[6][j])/sqrt(N-1);
		printf("Omega\n");
		printf("j=%d]-------------------------\n",j);
		printf("1] wait %f\n",omega[0][j]);
		printf("2] delay %f\n",omega[1][j]);
		printf("3] service %f\n",omega[2][j]);
		printf("4] numberNode %f\n",omega[3][j]);
		printf("5] numberQueue %f\n",omega[4][j]);
		printf("6] utilization %f\n",omega[5][j]);
		printf("7] job %f \n", omega[6][j]);

		printf("j=%d]-------------------------\n",j);
		printf("1] wait %f\n",somma1[j]/N);
		printf("2] delay %f\n",somma2[j]/N);
		printf("3] service %f\n",somma3[j]/N);
		printf("4] numberNode %f\n",somma4[j]/N);
		printf("5] numberQueue %f\n",somma5[j]/N);
		printf("6] utilization %f\n",somma6[j]/N);
		printf("7] job %f \n", (somma7[j])/N);
	}
}
void writeFileCSV(output matrix[N][12]){
	if(mkdir("./statistiche",0777)<0)
	{
		printf("error\n");
		//exit(-1);
	}
	
	FILE* file=fopen("./statistiche/transiente.csv","w+");
	if(file==NULL)
	{
		printf("error\n");
		exit(-1);
	}
	for(int j=0;j<12;j++){
		for(int i=0;i<N;i++){
			fprintf(file, "%f,",matrix[i][j].wait);
		}
		fprintf(file,"\n");
	}
	
	for(int j=0;j<12;j++){
		for(int i=0;i<N;i++){
			fprintf(file, "%f,",matrix[i][j].delay);
		}
		fprintf(file,"\n");
	}
	for(int j=0;j<12;j++){
		for(int i=0;i<N;i++){
			fprintf(file, "%f,",matrix[i][j].service);
		}
		fprintf(file,"\n");
	}
	for(int j=0;j<12;j++){
		for(int i=0;i<N;i++){
			fprintf(file, "%f,",matrix[i][j].numberNode);
		}
		fprintf(file,"\n");
	}
	for(int j=0;j<12;j++){
		for(int i=0;i<N;i++){
			fprintf(file, "%f,",matrix[i][j].numberQueue);
		}
		fprintf(file,"\n");
	}
	for(int j=0;j<12;j++){
		for(int i=0;i<N;i++){
			fprintf(file, "%f,",matrix[i][j].utilization);
		}
		fprintf(file,"\n");
	}
	for(int j=0;j<12;j++){
		for(int i=0;i<N;i++){
			fprintf(file, "%d,",matrix[i][j].job);
		}
		fprintf(file,"\n");
	}

}
int main(){ 
	//simulazione del transiente
	output matrix[N][12];
	double med[7][12];
	double var[7][12];
	double omega[7][12];
	PlantSeeds(SEED);
	azzeraOutput(matrix);
	for(int i=0;i<N;i++){
		simulatore(matrix[i],1440.0,1);	
	}
	writeFileCSV(matrix);
	media(matrix,med);
	varianza(matrix,med,var,omega);
	//simulazione dello stazionario



	return 0;
}



//
//  Main.c
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
		int n1=0;
		int n2=0;
		int n3=0;
		for(int i=0;i<N;i++)
		{
			if(matrix[i][j].wait==matrix[i][j].wait)
				somma1[j]=somma1[j]+matrix[i][j].wait;
			else
				n1++;
			if(matrix[i][j].delay==matrix[i][j].delay)
				somma2[j]=somma2[j]+matrix[i][j].delay;
			else
				n2++;
			if(matrix[i][j].service==matrix[i][j].service)
				somma3[j]=somma3[j]+matrix[i][j].service;
			else
				n3++;
			
			somma4[j]=somma4[j]+matrix[i][j].numberNode;
		
			somma5[j]=somma5[j]+matrix[i][j].numberQueue;
			
			somma6[j]=somma6[j]+matrix[i][j].utilization;

			somma7[j]=somma7[j]+matrix[i][j].job;
		}
		med[0][j]=somma1[j]/(N-n1);
		med[1][j]=somma2[j]/(N-n2);
		med[2][j]=somma3[j]/(N-n3);
		med[3][j]=somma4[j]/N;
		med[4][j]=somma5[j]/N;
		med[5][j]=somma6[j]/N;
		med[6][j]=somma7[j]/N;
		printf("j=%d]-------------------------\n",j);
		printf("1] wait %f\n",med[0][j]);
		printf("2] delay %f\n",med[1][j]);
		printf("3] service %f\n",med[2][j]);
		printf("4] numberNode %f\n",med[3][j]);
		printf("5] numberQueue %f\n",med[4][j]);
		printf("6] utilization %f\n",med[5][j]);
		printf("7] job %f \n", med[6][j]);
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
	int n1=0;
	int n2=0;
	int n3=0;
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
				n1++;

			if(matrix[i][j].delay==matrix[i][j].delay)
				somma2[j]=somma2[j]+(matrix[i][j].delay-med[1][j])*(matrix[i][j].delay-med[1][j]);
			else
				n2++;

			if(matrix[i][j].service==matrix[i][j].service)
				somma3[j]=somma3[j]+(matrix[i][j].service-med[2][j])*(matrix[i][j].service-med[2][j]);
			else
				n3++;

			somma4[j]=somma4[j]+(matrix[i][j].numberNode-med[3][j])*(matrix[i][j].numberNode-med[3][j]);
		
			somma5[j]=somma5[j]+(matrix[i][j].numberQueue-med[4][j])*(matrix[i][j].numberQueue-med[4][j]);
			
			somma6[j]=somma6[j]+(matrix[i][j].utilization-med[5][j])*(matrix[i][j].utilization-med[5][j]);

			somma7[j]=somma7[j]+(matrix[i][j].job-med[6][j])*(matrix[i][j].job-med[6][j]);
		}
		var[0][j]=somma1[j]/(N-n1);
		var[1][j]=somma2[j]/(N-n2);
		var[2][j]=somma3[j]/(N-n3);
		var[3][j]=somma4[j]/N;
		var[4][j]=somma5[j]/N;
		var[5][j]=somma6[j]/N;
		var[6][j]=somma7[j]/N;	
		omega[0][j]=tstar*sqrt(var[0][j])/sqrt(N-n1-1);
		omega[1][j]=tstar*sqrt(var[1][j])/sqrt(N-n2-1);
		omega[2][j]=tstar*sqrt(var[2][j])/sqrt(N-n3-1);
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
		printf("1] wait %f\n",var[0][j]);
		printf("2] delay %f\n",var[1][j]);
		printf("3] service %f\n",var[2][j]);
		printf("4] numberNode %f\n",var[3][j]);
		printf("5] numberQueue %f\n",var[4][j]);
		printf("6] utilization %f\n",var[5][j]);
		printf("7] job %f \n", var[6][j]);
	}
}
void incrementalMean(output matrix[N][12],int j, int i, output *out){
	double sum_w=0.0;
	double sum_d=0.0;
	double sum_s=0.0;
	double sum_nn=0.0;
	double sum_nq=0.0;
	double sum_u=0.0;
	double sum_j=0.0;
	int n=0;
	for(int k=0;k<=i;k++){
		if(matrix[k][j].wait==matrix[k][j].wait){
			sum_w+=matrix[k][j].wait;
			sum_d+=matrix[k][j].delay;
			sum_s+=matrix[k][j].service;
			sum_nn+=matrix[k][j].numberNode;
			sum_nq+=matrix[k][j].numberQueue;
			sum_u+=matrix[k][j].utilization;
			sum_j+=matrix[k][j].job;
			n++;
		}
	}
	out->wait=sum_w/n;
	out->delay=sum_d/n;
	out->service=sum_s/n;
	out->numberNode=sum_nn/(i+1);
	out->numberQueue=sum_nq/(i+1);
	out->utilization=sum_u/(i+1);
	out->job=sum_j/(i+1);
}
void writeFileCSV(output matrix[N][12]){
	if(mkdir("./statistiche",0777)<0)
	{
		printf("error\n");
		//exit(-1);
	}
	output out;
	FILE* file=fopen("./statistiche/transiente.csv","w+");
	if(file==NULL)
	{
		printf("error\n");
		exit(-1);
	}
	fprintf(file, "j,i,wait,delay,service,n_node,n_queue,rho,job\n");
	for(int j=0;j<12;j++){
		for(int i=0;i<N;i++){
			incrementalMean(matrix,j,i,&out);
			fprintf(file, "%d,%d,%f,%f,%f,%f,%f,%f,%f\n",j,i,out.wait,out.delay,out.service,out.numberNode,out.numberQueue,out.utilization,out.job);
		}
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



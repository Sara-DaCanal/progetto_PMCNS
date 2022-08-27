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
#include <string.h>
#include <errno.h>
#include <dirent.h>

#include "Main.h"
#include "libreriaRand/rngs.h" //spostare le librerie
#include "libreriaRand/rvms.h" 
#include "librerieProgetto/struct.h"
#include "librerieProgetto/utils.h"
#include "Simulatore.h"



void check(double media[7][15]){

	double vettorePesi[12]={3.0,10.0,0.0,0.0,0.0,4.0,2.0,3.0,2.0,1.0,4.0,2.0};
	double vettoreTempiPrevisti[12]={3.0,0.0,0.0,0.0,0.0,30.0,60.0,30.0,60.0,120.0,30.0,60.0};	
	double sum=0;
	for (int i = 0; i < 12; i++)
	{
			sum += vettorePesi[i]*(-media[1][i]+vettoreTempiPrevisti[i]);
	}
	printf("\nil valore della soluzione é: %f\n",sum);
	int sum1=65000-SERVERSTRIAGE*1700-SERVERSRED*7000-SERVERSTRAUMA*4200-SERVERSMEDICAL*4200-SERVERSMINOR*2800;
	printf("il Resto del budget é: %d\n",sum1);
}
void check1(double media[7][15]){

	double vettorePesi[15]={3.0,10.0,0.0,0.0,0.0,5.0,3.0,2.0,3.5,2.5,2.0,1.0,4.0,3.0,2.0};
	double vettoreTempiPrevisti[15]={3.0,0.0,0.0,0.0,0.0,15.0,45.0,60.0,15.0,45.0,60.0,120.0,15.0,45.0,60.0};	
	double sum=0;
	for (int i = 0; i < 15; i++)
	{
			sum += vettorePesi[i]*(-media[1][i]+vettoreTempiPrevisti[i]);
	}
	printf("\nil valore della soluzione é: %f\n",sum);
	int sum1=65000-SERVERSTRIAGE*1700-SERVERSRED*7000-SERVERSTRAUMA*4200-SERVERSMEDICAL*4200-SERVERSMINOR*2800;
	printf("il Resto del budget é: %d\n",sum1);
}

void azzeraOutput(output matrix[][15],int dim){
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<dim;j++){
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
void azzeradecessi(double decessi[]){
	for(int i=0;i<N;i++){
		decessi[i]=0.0;
	}
}
void mediaDecessi(double decessi[]){
	double sum=0.0;
	int n=0;
	for(int i=0;i<N;i++){
		if(decessi[i]==decessi[i]){
			sum+=decessi[i];
			n++;
		}
	}
	printf("Le morti sono in media: %f\n", sum/n);
}
void media(output matrix[N][15], double med[7][15],int dim)
{
	double somma1[dim];
	double somma2[dim];
	double somma3[dim];
	double somma4[dim];
	double somma5[dim];
	double somma6[dim];
	double somma7[dim];

	for(int j=0;j<dim;j++)
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
	}
}

void varianza(output matrix[][15],double med[7][15],double var[7][15],double omega[7][15],int dim)
{
	double tstar=idfStudent(N-1,1-ALFA/2);
	double somma1[dim];
	double somma2[dim];
	double somma3[dim];
	double somma4[dim];
	double somma5[dim];
	double somma6[dim];
	double somma7[dim];
	int n1=0;
	int n2=0;
	int n3=0;
	for(int j=0;j<dim;j++)
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
		printf("\n");
		printf("\t------------%s------------\n",matrix[0][j].nome);
		printf("\t wait %f ± %f\n",med[0][j],omega[0][j]);
		printf("\t delay %f ± %f\n",med[1][j],omega[1][j]);
		printf("\t service %f ± %f\n",med[2][j],omega[2][j]);
		printf("\t numberNode %f ± %f\n",med[3][j],omega[3][j]);
		printf("\t numberQueue %f ± %f\n",med[4][j],omega[4][j]);
		printf("\t utilization %f ± %f\n",med[5][j],omega[5][j]);
		printf("\t job %f ± %f\n",med[6][j],omega[6][j]);
		printf("\t---------------------------------\n");
	}
}
void incrementalMean(output matrix[N][15],int j, int i, output *out){
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
void writeFileCSV(output matrix[N][15], char *path,int dim){
	DIR *dir=opendir("./statistiche");
	if(dir){
		closedir(dir);
	}
	else if(ENOENT==errno){
		if(mkdir("./statistiche",0777)<0)
		{
			printf("error\n");
			exit(-1);
		}
	}
	output out;
	FILE* file=fopen(path,"w+");
	if(file==NULL)
	{
		printf("error\n");
		exit(-1);
	}
	fprintf(file, "j,i,wait,delay,service,n_node,n_queue,rho,job\n");
	for(int j=0;j<dim;j++){
		for(int i=0;i<N;i++){
			incrementalMean(matrix,j,i,&out);
			fprintf(file, "%d,%d,%f,%f,%f,%f,%f,%f,%f\n",j,i,out.wait,out.delay,out.service,out.numberNode,out.numberQueue,out.utilization,out.job);
		}
	}
	fclose(file);

}


int main(){ 
	//simulazione del transiente
	printf("\n\n\n\t---------------------------------\n");
	printf("\t*** FINITE HORIZON SIMULATION ***\n");
	printf("\t---------------------------------\n");
	output matrix[N][15];
	double decessi[N];
	strcpy(matrix[0][0].nome,"Triage");
	strcpy(matrix[0][1].nome,"Codici Rossi");
	strcpy(matrix[0][2].nome,"Traumatologia");
	strcpy(matrix[0][3].nome,"Problemi minori");
	strcpy(matrix[0][4].nome,"Problemi medici");
	strcpy(matrix[0][5].nome,"Trauma giallo");
	strcpy(matrix[0][6].nome,"Trauma verde");
	strcpy(matrix[0][7].nome,"P. Minori giallo");
	strcpy(matrix[0][8].nome,"P. Minori verde");
	strcpy(matrix[0][9].nome,"P. Minori bianco");
	strcpy(matrix[0][10].nome,"P. Medici giallo");
	strcpy(matrix[0][11].nome,"P. Medici verde");

	double med[7][15];
	double var[7][15];
	double omega[7][15];
	PlantSeeds(SEED);
	azzeraOutput(matrix,12);
	azzeradecessi(decessi);
	for(int i=0;i<N;i++){
		simulatore(matrix,decessi,i,1);	
	}
	writeFileCSV(matrix, "./statistiche/transiente.csv",12);
	media(matrix,med,12);
	varianza(matrix,med,var,omega,12);
	mediaDecessi(decessi);
	check(med);
	//simulazione dello stazionario
	printf("\n\n\n\t-----------------------------------\n");
	printf("\t*** INFINITE HORIZON SIMULATION ***\n");
	printf("\t-----------------------------------\n");
	PlantSeeds(SEED);
	azzeraOutput(matrix,12);
	azzeradecessi(decessi);
	simulatore(matrix, decessi, numBatch-1, 0);
	writeFileCSV(matrix, "./statistiche/steady_state.csv",12);
	media(matrix,med,12);
	varianza(matrix,med,var,omega,12);
	
	mediaDecessi(decessi);
	check(med);




	//simulazione del transiente
	printf("\n\n\n\t---------------------------------\n");
	printf("\t*** FINITE HORIZON SIMULATION ***\n");
	printf("\t---------------------------------\n");
	output matrix2[N][15];
	strcpy(matrix2[0][0].nome,"Triage");
	strcpy(matrix2[0][1].nome,"Codici Rossi");
	strcpy(matrix2[0][2].nome,"Traumatologia");
	strcpy(matrix2[0][3].nome,"Problemi medici");
	strcpy(matrix2[0][4].nome,"Problemi minori");
	strcpy(matrix2[0][5].nome,"Trauma arancione");
	strcpy(matrix2[0][6].nome,"Trauma blu");
	strcpy(matrix2[0][7].nome,"Trauma verde");
	strcpy(matrix2[0][8].nome,"P. Minori arancione");
	strcpy(matrix2[0][9].nome,"P. Minori blu");
	strcpy(matrix2[0][10].nome,"P. Minori verde");
	strcpy(matrix2[0][11].nome,"P. Minori bianco");
	strcpy(matrix2[0][12].nome,"P. Medici arancione");
	strcpy(matrix2[0][13].nome,"P. Medici blu");
	strcpy(matrix2[0][14].nome,"P. Medici verde");
	double med2[7][15];
	double var2[7][15];
	double omega2[7][15];
	PlantSeeds(SEED);
	azzeraOutput(matrix2,15);
	azzeradecessi(decessi);
	for(int i=0;i<N;i++){
		simulatore2(matrix2,decessi,i,1);	
	}
	writeFileCSV(matrix2, "./statistiche/transiente_migliorato.csv",15);
	media(matrix2,med2,15);
	varianza(matrix2,med2,var2,omega2,15);
	mediaDecessi(decessi);
	check1(med2);
	//simulazione dello stazionario
	printf("\n\n\n\t-----------------------------------\n");
	printf("\t*** INFINITE HORIZON SIMULATION ***\n");
	printf("\t-----------------------------------\n");
	PlantSeeds(SEED);
	azzeraOutput(matrix2,15);
	simulatore2(matrix2, decessi, numBatch-1, 0);
	writeFileCSV(matrix2, "./statistiche/steady_state_migliorato.csv",15);
	media(matrix2,med2,15);
	varianza(matrix2,med2,var2,omega2,15);
	mediaDecessi(decessi);
	check1(med2);
	return 0;
}



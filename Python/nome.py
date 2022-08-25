import numpy as np
import matplotlib.pyplot as plt
import matplotlib.lines as lines
import os 
import sys
import csv

theoricDelay=[2.5392,2.27991,27.05871,33.4121,73.32531,12.70077,31.41034,8.28938,16.6594,69.7234,23.82312,88.32865]
theoricWait=[12.5392,227.77991,120.45871,138.7221,239.22531,106.10077,122.81034,115.0,120,180,189.72312,254.22865]
theoricService=[10.0,225.5,93.4,105.6,165.9,93.4,93.4,105.6,105.6,105.6,165.9,165.9]
theoricNum_node=[]
theoricNum_Queue=[]
theoricRho=[0.45,0.10,0.59565,0.790137,0.73029,0.23259,0.45711,0.12139,0.40052,0.26946,0.16986,0.56043]
theoricJob=[]
pathTransiente="./../Simulatore_ProntoSoccorso/statistiche/transiente.csv"
pathStazionario="./../Simulatore_ProntoSoccorso/statistiche/steady_state.csv"
def grafici(path):
	csvFile = open(path,newline="\n")
	spamreader = csv.reader(csvFile, delimiter=',');
	delay=[];
	wait=[];
	service=[];
	num_node=[];
	num_queue=[];
	rho=[];
	job=[];

	delay.append([])
	wait.append([])
	service.append([])
	num_node.append([])
	num_queue.append([])
	rho.append([])
	job.append([])
	k=0
	count=0;
	for i in spamreader:
		if count!=0:
			if int(i[0])!=k:
				k=k+1
				delay.append([])
				wait.append([])
				service.append([])
				num_node.append([])
				num_queue.append([])
				rho.append([])
				job.append([])
			wait[k].append(float(i[2]))
			delay[k].append(float(i[3]))
			service[k].append(float(i[4]))
			num_node[k].append(float(i[5]))
			num_queue[k].append(float(i[6]))
			rho[k].append(float(i[7]))
			job[k].append(float(i[8]))
		count=count+1

	for i in range(0,12):
		plt.figure("node "+str(i+1))
		
		ax=plt.subplot(4,1,1)
		plt.title("wait")
		plt.plot(wait[i],"or--",label='wait')
		plt.plot([theoricWait[i]]*64,"-",color="y",label='theorical Wait')
		ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(1, 0.5))
		
		ax=plt.subplot(4,1,2)
		plt.title("delay")
		plt.plot(delay[i],"ob--",label='delay')
		plt.plot([theoricDelay[i]]*64,"-",color="y",label='theorical delay')
		ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(1, 0.5))
		
		ax=plt.subplot(4,1,3)
		plt.title("service")
		plt.plot(service[i],"og--",label='service')
		plt.plot([theoricService[i]]*64,"-",color="y",label='theorical service')
		ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(1, 0.5))
		
		ax=plt.subplot(4,1,4)
		plt.title("Rho")
		plt.plot(rho[i],"og--", label='rho')
		plt.plot([theoricRho[i]]*64,"-",color="y", label='theorical rho')
		ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(1, 0.5))
		
		plt.show()
		
grafici(pathTransiente)
grafici(pathStazionario)
'''
x=f.read().split()
for string in x:
	print("stringa singola")
	print(string.split())
'''

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.lines as lines
import os 
import sys
import csv

NFILE=64
theoricDelay=[2.5392,2.53958,26.26619,33.4121,73.32531,12.43217,30.45907,8.28938,16.6594,69.7234,23.82312,88.32865]
theoricWait=[12.5392,228.03958,119.66619,138.7221,239.22531,105.83217,123.85907,115.0,120,180,189.72312,254.22865]
theoricService=[10.0,225.5,93.4,105.6,165.9,93.4,93.4,105.6,105.6,105.6,165.9,165.9]

theoricDelay=[2.5392,2.53958,26.26619,73.32531,36.35195,12.43217,30.45907,23.82312,88.32865,8.28938,18.5823,75.40605]

theoricWait=[12.5392,228.03958,119.66619,239.22531,141.195,105.83217,123.85907,189.72312,254.22865,113.88938,124.18230,181.00605]

theoricRho=[0.45,0.10553,0.59184,0.73029,0.79137,0.13766,0.45418,0.16986,0.56043,0.12139,0.40052,0.26946]


theoricNum_node=[]
theoricNum_Queue=[]
theoricRho=[0.45,0.10553,0.59184,0.790137,0.73029,0.13766,0.45418,0.12139,0.40052,0.26946,0.16986,0.56043]

theoricRho=[0.45,0.10553,0.59184,0.790137,0.73029,0.13766,0.45418,0.16986,0.56043,0.12139,0.40052,0.26946]

theoricRho=[0.45,0.10553,0.59184,0.73029,0.79137,0.13766,0.45418,0.16986,0.56043,0.12139,0.40052,0.26946]

theoricJob=[]

theoric2Delay=[2.5392,2.53958,26.26619,33.4121,73.32531,12.43217,30.45907,8.28938,16.6594,69.7234,23.82312,88.32865]
theoric2Wait=[12.5392,228.03958,119.66619,138.7221,239.22531,105.83217,123.85907,115.0,120,180,189.72312,254.22865]
theoric2Service=[10.0,225.5,93.4,105.6,165.9,93.4,93.4,105.6,105.6,105.6,165.9,165.9]
theoric2Num_node=[]
theoric2Num_Queue=[]
theoric2Rho=[0.45,0.10553,0.59184,0.790137,0.73029,0.13766,0.45418,0.12139,0.40052,0.26946,0.16986,0.56043]
theoric2Job=[]



pathTransiente="./../Simulatore_ProntoSoccorso/statistiche/transiente.csv"
pathStazionario="./../Simulatore_ProntoSoccorso/statistiche/steady_state.csv"
pathTransienteConfidenza="./../Simulatore_ProntoSoccorso/statistiche/intervalli_di_confidenzainFinite.csv"
pathStazionarioConfidenza="./../Simulatore_ProntoSoccorso/statistiche/intervalli_di_confidenzainFinite.csv"


pathStazionario2="./../Simulatore_ProntoSoccorso/statistiche/pippo0.csv"
DueDelay=[3.0,0.0,0.0,0.0,0.0,30.0,60.0,30.0,60.0,120.0,30.0,60.0]
def graficiPrimaParte(path,confidenzaPath):
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

	ax=plt.subplot(1,1,1);
	plt.title("Delay Triage")
	plt.plot(delay[0],"or--",label='triage')
	plt.plot(funcM(delay[0]),"or",label="intervalUp")
	plt.plot(funcm(delay[0]),"or",label="intervalUp")
	plt.plot([theoricDelay[0]]*64,"c-",label='theorical delay')
	plt.xlabel('Batch')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	plt.show()

	ax=plt.subplot(1,1,1);
	plt.title("Delay RedCode")
	plt.plot(delay[1],"or--",label='redCode');
	plt.plot([DueDelay[1]]*64,"k-",label='Due Delay')
	#plt.plot([theoricDelay[1]]*64,"c-",label='theorical delay')
	plt.xlabel('Batch')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))

	plt.show()

	ax=plt.subplot(2,1,1);
	plt.title("Delay Trauma")
	plt.plot(delay[5],"oy--",label='Trauma Yellow');
	plt.plot([DueDelay[5]]*64,"k-",label='Due Delay yellow')
	#plt.plot([theoricDelay[5]]*64,"c-",label='theorical delay yellow')
	plt.xlabel('Batch')
	plt.ylabel('minutes')	
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))

	ax=plt.subplot(2,1,2);
	plt.plot(delay[6],"og--",label='Trauma green');
	plt.plot([DueDelay[6]]*64,"k-",label='Due Delay green')
	#plt.plot([theoricDelay[6]]*64,"c-",label='theorical delay green')
	plt.xlabel('Batch')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	plt.show()

	ax=plt.subplot(2,1,1);
	plt.title("Delay Medical")
	plt.plot(delay[10],"oy--",label='Medical Yellow');
	plt.plot([DueDelay[10]]*64,"k-",label='Due Delay yellow')
	#plt.plot([theoricDelay[10]]*64,"c-",label='theorical delay yellow')	
	plt.xlabel('Batch')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	
	ax=plt.subplot(2,1,2);
	plt.plot(delay[11],"og--",label='Medical green');
	plt.plot([DueDelay[11]]*64,"k-",label='Due Delay green')
	#plt.plot([theoricDelay[11]]*64,"c-",label='theorical delay green')
	plt.xlabel('Batch')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	plt.show()

	ax=plt.subplot(3,1,1);
	plt.title("Delay Minor")
	plt.plot(delay[7],"oy--",label='Minor Yellow');
	plt.plot([DueDelay[7]]*64,"k-",label='Due Delay yellow')
	#plt.plot([theoricDelay[7]]*64,"c-",label='theorical delay yellow')	
	plt.xlabel('Batch')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	
	ax=plt.subplot(3,1,2);
	plt.plot(delay[8],"og--",label='Minor green');
	plt.plot([DueDelay[8]]*64,"k-",label='Due Delay green')
	#plt.plot([theoricDelay[8]]*64,"c-",label='theorical delay green')
	plt.xlabel('Batch')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))


	ax=plt.subplot(3,1,3);
	plt.plot(delay[9],"o--",color="grey",label='Minor white');
	plt.plot([DueDelay[9]]*64,"k-",label='Due Delay white')
	#plt.plot([theoricDelay[9]]*64,"c-",label='theorical delay white')
	plt.xlabel('Batch')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	plt.show()

	
	'''
	ax=plt.title("Delay RedCode")
	plt.plot(delay[1],"or--",label='redCode');
	plt.plot([DueDelay[1]]*64,"k-",label='Due Delay')
	plt.plot([theoricDelay[1]]*64,"c-",color="y",label='theorical Wait')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(1, 0.5))

	ax=plt.title("Delay Triage")
	plt.plot(delay[0],"or--",label='triage');
	plt.plot([DueDelay[0]]*64,"k-",label='Due Delay')
	plt.plot([theoricDelay[0]]*64,"c-",color="y",label='theorical Wait')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(1, 0.5))

	ax=plt.title("Delay RedCode")
	plt.plot(delay[1],"or--",label='redCode');
	plt.plot([DueDelay[1]]*64,"k-",label='Due Delay')
	plt.plot([theoricDelay[1]]*64,"c-",color="y",label='theorical Wait')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(1, 0.5))
	'''

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

def graficiIntervalConfidence(path,path2):
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
	csvFile = open(path2,newline="\n")
	spamreader = csv.reader(csvFile, delimiter=',');
	nome=[]
	confidenzadelay=[];
	confidenzawait=[];
	confidenzaservice=[];
	confidenzanum_node=[];
	confidenzanum_queue=[];
	confidenzarho=[];
	confidenzajob=[];
	k=0
	for i in spamreader:
		if k!= 0:
			nome.append(i[0])
			confidenzawait.append(float(i[1]))
			confidenzadelay.append(float(i[2]))
			confidenzaservice.append(float(i[3]))
			confidenzanum_node.append(float(i[4]))
			confidenzanum_queue.append(float(i[5]))
			confidenzarho.append(float(i[6]))
			confidenzajob.append(float(i[7]))
		k=k+1;

	k=0;
	for n in nome:
		x=[0]*64
		for i in range(0,64):
			x[i]=i
		fig, ax = plt.subplots()
		fig.suptitle("delay Triage"+n)
		ax.plot(delay[k],"or--",label=n)
		ax.plot([theoricDelay[k]]*64,"c-",label="theorical delay "+n)
		delaym=[0]*64;
		delayp=[0]*64;
		i=0
		for x in delay[k]:
			delaym[i]=(x-confidenzadelay[k])
			delayp[i]=(x+confidenzadelay[k])
			i=i+1

		ax.plot(delaym,"-b",label="intervalDown")
		ax.plot(delayp,"-b",label="intervalUp")
		plt.xlabel('Batch')
		plt.ylabel('minutes')
		ax.legend(title='Parameter where:')
		plt.show()

		
		x=[0]*64
		for i in range(0,64):
			x[i]=i
		fig, ax = plt.subplots()
		fig.suptitle("rho Triage"+n)
		ax.plot(rho[k],"or--",label=n)
		ax.plot([theoricRho[k]]*64,"c-",label="theorical rho "+n)
		rhom=[0]*64;
		rhop=[0]*64;
		i=0
		for x in rho[k]:
			rhom[i]=(x-confidenzarho[k])
			rhop[i]=(x+confidenzarho[k])
			i=i+1

		ax.plot(rhom,"-b",label="intervalDown")
		ax.plot(rhop,"-b",label="intervalUp")
		plt.xlabel('Batch')
		plt.ylabel('%')
		ax.legend(title='Parameter where:')
		#ax.set_ylim([0.4, 0.5])
		plt.show()

		k=k+1


def grafici2LaVendetta(path):
	csvFile = open(path,newline="\n")
	spamreader = csv.reader(csvFile, delimiter=',');
	delay=[];
	wait=[];
	rho=[];
	for j in range(0,NFILE):
		delay.append([])
		wait.append([])
		rho.append([])
		for i in range(0,12):
			delay[j].append([])
			wait[j].append([])
			rho[j].append([])
	for k in range(0,NFILE):
		path="./../Simulatore_ProntoSoccorso/statistiche/pippo"+str(k)+".csv"
		count=0;
		csvFile = open(path,newline="\n")
		spamreader = csv.reader(csvFile, delimiter=',');
		for i in spamreader:
			if count!=0:
				if i[0]=="Triage":
					wait[k][0].append(float(i[3]))
					delay[k][0].append(float(i[4]))
					rho[k][0].append(float(i[8]))
				if i[0]=="Codici Rossi":
					wait[k][1].append(float(i[3]))
					delay[k][1].append(float(i[4]))
					rho[k][1].append(float(i[8]))
				if i[0]=="Traumatologia":
					wait[k][2].append(float(i[3]))
					delay[k][2].append(float(i[4]))
					rho[k][2].append(float(i[8]))
				if i[0]=="Problemi medici":
					wait[k][3].append(float(i[3]))
					delay[k][3].append(float(i[4]))
					rho[k][3].append(float(i[8]))
				if i[0]=="Problemi minori":
					wait[k][4].append(float(i[3]))
					delay[k][4].append(float(i[4]))
					rho[k][4].append(float(i[8]))
				if i[0]=="Trauma giallo":
					wait[k][5].append(float(i[3]))
					delay[k][5].append(float(i[4]))
					rho[k][5].append(float(i[8]))
				if i[0]=="Trauma verde":
					wait[k][6].append(float(i[3]))
					delay[k][6].append(float(i[4]))
					rho[k][6].append(float(i[8]))
				if i[0]=="P. Medici giallo":
					wait[k][7].append(float(i[3]))
					delay[k][7].append(float(i[4]))
					rho[k][7].append(float(i[8]))
				if i[0]=="P. Medici verde":
					wait[k][8].append(float(i[3]))
					delay[k][8].append(float(i[4]))
					rho[k][8].append(float(i[8]))
				if i[0]=="P. Minori giallo":
					wait[k][9].append(float(i[3]))
					delay[k][9].append(float(i[4]))
					rho[k][9].append(float(i[8]))
				if i[0]=="P. Minori verde":
					wait[k][10].append(float(i[3]))
					delay[k][10].append(float(i[4]))
					rho[k][10].append(float(i[8]))
				if i[0]=="P. Minori bianco":
					wait[k][11].append(float(i[3]))
					delay[k][11].append(float(i[4]))
					rho[k][11].append(float(i[8]))
			count=count+1
	minDim=len(wait[0][0])
	for i in range(0,64):
		if minDim>len(wait[i][0]):
			minDim=len(wait[i][0])
	print(minDim)
	
	vettoreReparti=["Triage","Codice Rosso","Traumatologia","Problemi medici","Problemi minori","Trauma giallo","Trauma verde","P.medici giallo","P.medici verde","P.minori giallo","P.minori verede","P.miori bianco"]
	
	vettoreDelay=[]
	for k in range(0,12):
		vettoreDelay.append([])
		for i in range(0,minDim):
			sum=0;
			for j in range(0,NFILE):
					sum+=delay[j][k][i]
			vettoreDelay[k].append(sum/(64.0))

	
	ax=plt.subplot(1,1,1);
	plt.title("Delay Triage")
	plt.plot(vettoreDelay[0],"r-",label='triage')
	plt.plot([DueDelay[0]]*minDim,"c-",label='theorical delay')
	plt.xlabel('job')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	plt.savefig("Delay"+vettoreReparti[0]+"3.png",format="png", dpi=150)
	plt.show()

	ax=plt.subplot(1,1,1);
	plt.title("Delay RedCode")
	plt.plot(vettoreDelay[1],"r-",label='redCode');
	plt.plot([DueDelay[1]]*minDim,"k-",label='Due Delay')
	plt.xlabel('job')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	plt.savefig("Delay"+vettoreReparti[1]+"3.png",format="png", dpi=150)
	plt.show()

	ax=plt.subplot(2,1,1);
	plt.title("Delay Trauma")
	plt.plot(vettoreDelay[5],"y-",label='Trauma Yellow');
	plt.plot([DueDelay[5]]*minDim,"k-",label='Due Delay yellow')
	plt.xlabel('job')
	plt.ylabel('minutes')	
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))

	ax=plt.subplot(2,1,2);
	plt.plot(vettoreDelay[6],"g-",label='Trauma green');
	plt.plot([DueDelay[6]]*minDim,"k-",label='Due Delay green')
	plt.xlabel('job')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	plt.savefig("Delay"+vettoreReparti[2]+"3.png",format="png", dpi=150)
	plt.show()

	ax=plt.subplot(2,1,1);
	plt.title("Delay Medical")
	plt.plot(vettoreDelay[7],"y-",label='Medical Yellow');
	plt.plot([DueDelay[10]]*minDim,"k-",label='Due Delay yellow')
	plt.xlabel('job')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	
	ax=plt.subplot(2,1,2);
	plt.plot(vettoreDelay[8],"g-",label='Medical green');
	plt.plot([DueDelay[11]]*minDim,"k-",label='Due Delay green')

	plt.xlabel('job')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	plt.savefig("Delay"+vettoreReparti[3]+"3.png",format="png", dpi=150)
	plt.show()

	ax=plt.subplot(3,1,1);
	plt.title("Delay Minor")
	plt.plot(vettoreDelay[9],"y-",label='Minor Yellow');
	plt.plot([DueDelay[7]]*minDim,"k-",label='Due Delay yellow')
	plt.xlabel('job')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	
	ax=plt.subplot(3,1,2);
	plt.plot(vettoreDelay[10],"g-",label='Minor green');
	plt.plot([DueDelay[8]]*minDim,"k-",label='Due Delay green')
	plt.xlabel('job')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))


	ax=plt.subplot(3,1,3);
	plt.plot(vettoreDelay[11],"-",color="grey",label='Minor white');
	plt.plot([DueDelay[9]]*minDim,"k-",label='Due Delay white')
	plt.xlabel('job')
	plt.ylabel('minutes')
	ax.legend(title='Parameter where:',loc='center left',bbox_to_anchor=(0.8, 0.5))
	plt.savefig("Delay"+vettoreReparti[4]+"3.png",format="png", dpi=150)
	plt.show()


	'''
	for i in range(0,12):
		plt.title("wait "+vettoreReparti[i])
		plt.plot(wait[0][i],"r--",label='wait')
		plt.plot(wait[1][i],"b--",label='wait')
		plt.plot(wait[2][i],"g--",label='wait')
		plt.plot(wait[3][i],"y--",label='wait')
		plt.plot(wait[4][i],"c--",label='wait')
		plt.xlabel("job")
		plt.ylabel("minutes")
		plt.plot([theoricWait[i]]*max_number,"k-",label="theorical")
		#plt.savefig("wait"+vettoreReparti[i]+"2.png",format="png", dpi=150)
		plt.show()

	for i in range(0,12):
		plt.title("delay "+vettoreReparti[i])
		plt.plot(delay[0][i],"r--",label='wait')
		#plt.plot(delay[1][i],"b--",label='wait')
		#plt.plot(delay[2][i],"g--",label='wait')
		#plt.plot(delay[3][i],"y--",label='wait')
		#plt.plot(delay[4][i],"c--",label='wait')
		plt.ylabel("minutes")
		plt.xlabel("job")
		plt.plot([theoricDelay[i]]*max_number,"k-",label="theorical")
		#plt.savefig("Delay"+vettoreReparti[i]+"2.png",format="png", dpi=150)
		plt.show()

	for i in range(0,12):
		plt.title("Rho "+vettoreReparti[i])
		plt.plot(rho[0][i],"r--",label='wait')
		#plt.plot(rho[1][i],"b--",label='wait')
		#plt.plot(rho[2][i],"g--",label='wait')
		#plt.plot(rho[3][i],"y--",label='wait')
		#plt.plot(rho[4][i],"c--",label='wait')
		plt.ylabel("minutes")
		plt.xlabel("job")
		plt.plot([theoricRho[i]]*max_number,"k-",label="theorical")
		#plt.savefig("rho"+vettoreReparti[i]+"2.png",format="png", dpi=150)
		plt.show()
	'''		

		



grafici2LaVendetta(pathStazionario2)

#graficiIntervalConfidence(pathStazionario,pathStazionarioConfidenza)
#graficiPrimaParte(pathStazionario);
#grafici(pathTransiente)
#grafici(pathStazionario)

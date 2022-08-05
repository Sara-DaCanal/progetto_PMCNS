//
//  Main.c
//  Simulatore_ProntoSoccorso
//
//  Created by Matteo Federico on 12/05/22.
//

//7

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libreriaRand/rngs.h" //spostare le librerie
#define START 0.0
#define STOP 20000.0
#define INF   (10.0 * STOP)
#define SEED 123456789
#define SERVERSTRIAGE 4
#define SERVERSRED 3

#define SERVERSTRAUMA 3     //@ADDED
#define SERVERSMEDICAL 3    //@ADDED
#define SERVERSMINOR 3      //@ADDED

double arrival;


typedef struct event{
    double arrival;
    int completionTriage; //indice del primo server che finisce
    int completionRossa;
    int completionTrauma;
    int completionMedical;
    int completionMinor;
    double current;
    double next;
    double last;
}event;

typedef enum color{
    white,
    green,
    yellow,
    red,
    none
}color;

typedef struct multiserver{
    double service;
    int occupied;
    color color;
}multiserver;

typedef struct paramService{
    double mean;
    int stream;
}paramService;



paramService traumaParams;
paramService medicalParams;
paramService minorParams;
paramService triageParams;
paramService redParams;

void intitParamService(){
    traumaParams.mean=93.4*SERVERSTRAUMA;
    medicalParams.mean=165.9*SERVERSMEDICAL;
    minorParams.mean=105.6*SERVERSMINOR;
    redParams.mean=225.5*SERVERSRED;
    triageParams.mean=6*SERVERSTRIAGE;

    triageParams.stream=0;
    redParams.stream=1;
    traumaParams.stream=2;
    medicalParams.stream=3;
    minorParams.stream=4;
}

double Exponential(double m)
/* ---------------------------------------------------
 * generate an Exponential random variate, use m > 0.0 
 * ---------------------------------------------------
 */
{
  return (-m * log(1.0 - Random()));
}
double Uniform(double a, double b)
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{
  return (a + (b - a) * Random());
}
double Min(double a, multiserver triage[], multiserver rossa[])
/* ------------------------------
 * return the smaller of a, b
 * ------------------------------
 */
{ 
    double min=a;
    for(int i=0;i<SERVERSTRIAGE;i++){
        if(triage[i].service < min)
            min = triage[i].service;
    }
    int j=-1;
    for(int i=0;i<SERVERSRED;i++){
        if(rossa[i].service < min){
            min = rossa[i].service;
            j=i;
        }
    }
    printf("J è:%d\n", j);
    return min;
}

double getArrival(){
    SelectStream(5);
    arrival+=Exponential(1/0.09);
    return arrival;
}

 int NextEvent(multiserver event[], int length)
/* ---------------------------------------
 * return the index of the next event type
 * ---------------------------------------
 */
{
  int e = 0;                                      
  int i = 1;
                    
  while (i < length) {         /* now, check the others to find which  */
                         /* event type is most imminent          */
    if ((event[i].occupied == 1) && (event[i].service < event[e].service))
      e = i;
    i++; 
  }
  if(event[e].occupied==0){
    printf("NextEvent: -1\n");
    return -1;
  }
  printf("NextEvent: %d\n", e);
  return (e);
}


 int FindOne(multiserver event[])
/* -----------------------------------------------------
 * return the index of the available server idle longest
 * -----------------------------------------------------
 */
{
    int s;
    int i = 0;

     while (event[i].occupied == 1)       /* find the index of the first available */
        i++;                        /* (idle) server                         */
    s = i;
    return (s);
}

color assignCode(){
    SelectStream(6);
    double x = Uniform(0,100);
    if (x<10.04)
        return red;
    else if (x<19.44)
        return yellow;
    else if (x<80.15)
        return green;
    else
        return white;
}

/*double getServiceTriage(){
  SelectStream(1);
  return Exponential(24);                  //La media del servizio è di sei minuti, quindi ogni server ha media 4*6=24
}
double getServiceRossa(){
    SelectStream(3);
    return Exponential(225.5);
}*/

double getService(paramService service){
    SelectStream(service.stream);
    return Exponential(service.mean);
}


int main(){
    intitParamService();
    arrival=START;
    int counter = 0;
    int numeroCodaTriage=0;
    int numeroCodaRossa=0;

    int numMedicoGiallo=0;
    int numMedicoVerde=0;

    int numMinoreGiallo=0;
    int numMinoreVerde=0;
    int numMinoreBianco=0;

    int numTraumiGiallo=0;
    int numTraumiVerde=0;

    int inServiceYellowTrauma;
    int inServiceYellowMedical;
    int inServiceYellowMinor;

    int inServiceGreenMinor;

    PlantSeeds(SEED);
    event t;
    multiserver triage[SERVERSTRIAGE];      //inizializzare array per salvare il multiserver
    multiserver rossa[SERVERSRED];

    multiserver traumi[SERVERSTRAUMA];      //inizializzare array per salvare il multiserver
    multiserver medico[SERVERSMEDICAL];
    multiserver minori[SERVERSMINOR];      //inizializzare array per salvare il multiserver

    for(int i=0; i<SERVERSTRIAGE; i++){ 
        triage[i].occupied=0;
        triage[i].service=INF;
    }

    for(int i=0; i<SERVERSMEDICAL; i++){ 
        medico[i].occupied=0;
        medico[i].service=INF;
    }

    for(int i=0; i<SERVERSMINOR; i++){ 
        minori[i].occupied=0;
        minori[i].service=INF;
    }

    for(int i=0; i<SERVERSTRAUMA; i++){ 
        traumi[i].occupied=0;
        traumi[i].service=INF;
    }

    for(int i=0;i<SERVERSRED; i++){
        rossa[i].occupied=0;
        rossa[i].service=INF;
    }
    t.arrival=getArrival();
    t.completionTriage=-1;
    t.completionRossa=-1;
    t.current=START;
    t.last=START;   //per statistiche non serve

    while(t.arrival<STOP || numeroCodaTriage>0 || numeroCodaRossa>0)
    {
        t.next=Min(t.arrival, triage, rossa);    //probabilmente non serve poi vediamo
        t.current=t.next;
        if(t.current==t.arrival){
            numeroCodaTriage++;
            t.arrival=getArrival();
            if(t.arrival>STOP)
            {
                t.last=t.current;
                t.arrival=INF;
            }
            if(numeroCodaTriage<=SERVERSTRIAGE){
                int index = FindOne(triage); //trovare un server libero
                triage[index].service=t.current+getService(triageParams);
                triage[index].occupied=1;
                t.completionTriage=NextEvent(triage, SERVERSTRIAGE);
            }
        }
        else if(t.current==triage[t.completionTriage].service){
               numeroCodaTriage--;
               if(numeroCodaTriage-SERVERSTRIAGE>=0)
               {
                    triage[t.completionTriage].service=t.current+getService(triageParams);
                    triage[t.completionTriage].occupied=1;
                    
               }
               else{
                    triage[t.completionTriage].service=INF;
                    triage[t.completionTriage].occupied=0;
               }
               if(numeroCodaTriage > 0)
                    t.completionTriage=NextEvent(triage, SERVERSTRIAGE);
                else
                    t.completionTriage=-1;
               color code = assignCode();
               double p;    //p è usato per le probabilità
               switch (code)
               {
                case red:
                    numeroCodaRossa++;
                    counter++;
                    if(numeroCodaRossa<=SERVERSRED){
                        int index=FindOne(rossa);
                        rossa[index].service = t.current+getService(redParams);
                        rossa[index].occupied = 1;
                        t.completionRossa = NextEvent(rossa, SERVERSRED);
                    }
                    /*Probabilità muoia qualcuno*/
                    else{
                        SelectStream(7);
                        p = Uniform(0,100);
                        if(p<5.2){              //costante da decidere
                            numeroCodaRossa--;
                            printf("1 Killed!\n");
                        }
                    }
                break;
                case yellow:
                    SelectStream(8);
                    p = Uniform(0,100);
                    if(p<26.7){              //@costante da fixare, va a finire in traumi
                            numTraumiGiallo++;
                            if(numTraumiGiallo+numTraumiVerde<=SERVERSTRAUMA){
                                inServiceYellowTrauma++;
                                int index = FindOne(traumi); //trovare un server libero
                                traumi[index].service=t.current+getService(traumaParams); 
                                traumi[index].occupied=1;
                                traumi[index].color=yellow;
                                t.completionTrauma=NextEvent(traumi, SERVERSTRAUMA);
                            }
                    }
                    if(p<51.4){             //@costante da fixare, va a finire in problema medico
                            numMedicoGiallo++;
                            if(numMedicoGiallo+numMedicoVerde<=SERVERSMEDICAL){
                                inServiceYellowMedical++;
                                int index = FindOne(medico); //trovare un server libero
                                medico[index].service=t.current+getService(medicalParams); 
                                medico[index].occupied=1;
                                medico[index].color=yellow;
                                t.completionMedical=NextEvent(medico, SERVERSMEDICAL);
                            }
                    }
                    else{    numMinoreGiallo++; 
                             if(numMinoreGiallo+numMinoreVerde+numMinoreBianco<=SERVERSMINOR){
                                inServiceYellowMinor++;
                                int index = FindOne(minori); //trovare un server libero
                                minori[index].service=t.current+getService(minorParams); 
                                minori[index].occupied=1;
                                minori[index].color=yellow;
                                t.completionMinor=NextEvent(minori, SERVERSMINOR);
                            }
                    }
                    break;


                case green:
                    SelectStream(9);
                    p = Uniform(0,100);
                    if(p<26.7){             
                            numTraumiVerde++;
                            if(numTraumiGiallo+numTraumiVerde<=SERVERSTRAUMA){
                                if(numTraumiGiallo-inServiceYellowTrauma==0){
                                    int index = FindOne(traumi); //trovare un server libero
                                    traumi[index].service=t.current+getService(traumaParams); 
                                    traumi[index].occupied=1;
                                    minori[index].color=green;
                                    t.completionTrauma=NextEvent(traumi, SERVERSTRAUMA);
                                }
                            }
                    }
                    if(p<51.4){             
                            numMedicoVerde++;
                            if(numMedicoGiallo+numMedicoVerde<=SERVERSMEDICAL){
                                if(numMedicoGiallo-inServiceYellowMedical==0){
                                    int index = FindOne(medico); //trovare un server libero
                                    medico[index].service=t.current+getService(medicalParams); 
                                    medico[index].occupied=1;
                                    minori[index].color=green;
                                    t.completionMedical=NextEvent(medico, SERVERSMEDICAL);
                                }
                            }
                    }
                    else{    numMinoreVerde++; 
                             if(numMinoreGiallo+numMinoreVerde+numMinoreBianco<=SERVERSMINOR){

                                if(numMinoreGiallo-inServiceYellowMinor==0){
                                    inServiceGreenMinor++;
                                    int index = FindOne(minori); //trovare un server libero
                                    minori[index].service=t.current+getService(minorParams); 
                                    minori[index].occupied=1;
                                    minori[index].color=green;
                                    t.completionMinor=NextEvent(minori, SERVERSMINOR);
                                }
                            }
                    }
                    break;
                case white: 
                    numMinoreBianco++;
                    if(numMinoreGiallo+numMinoreVerde+numMinoreBianco<=SERVERSMINOR){

                                if((numMinoreGiallo-inServiceYellowMinor==0) && (numMinoreVerde-inServiceGreenMinor==0)){
                                    int index = FindOne(minori); //trovare un server libero
                                    minori[index].service=t.current+getService(minorParams); 
                                    minori[index].occupied=1;
                                    minori[index].color=white;
                                    t.completionMinor=NextEvent(minori, SERVERSMINOR);
                                }
                    }
                
                break; //@va a finire per forza nei problemi minori
               default:
                break;
               }
               }
               //else if(t.current=)
               else if(t.current==rossa[t.completionRossa].service){
                    numeroCodaRossa--;
                    if(numeroCodaRossa-SERVERSRED>=0)
                    {
                    rossa[t.completionRossa].service=t.current+getService(redParams);
                    rossa[t.completionRossa].occupied=1;
                    
               }
               else{
                    rossa[t.completionRossa].service=INF;
                    rossa[t.completionRossa].occupied=0;
               }
               if(numeroCodaRossa > 0)
                    t.completionRossa=NextEvent(rossa, SERVERSRED);
                else
                    t.completionRossa=-1;

               }
               else{
                //eventi futuri dei completamenti degli altri reparti
               } 
               t.completionRossa=NextEvent(rossa, SERVERSRED);
               t.completionTriage=NextEvent(triage, SERVERSTRIAGE);
               //printf("Tempo corrente:%f,\ttempo arrivo:%f\tcompl triage:%f,\tcompl rossa:%f\n", t.current, t.arrival, triage[t.completionTriage].service, rossa[t.completionRossa].service);      
    }
    printf("Rossi: %d\n",counter);
    return 0;
    }


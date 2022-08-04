//
//  Main.c
//  Simulatore_ProntoSoccorso
//
//  Created by Matteo Federico on 12/05/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libreriaRand/rngs.h" //spostare le librerie
#define START 0.0
#define STOP 2000.0
#define INF   (100.0 * STOP)
#define SEED 123456789
#define SERVERSTRIAGE 4
#define SERVERSRED 3

double arrival;

typedef struct event{
    double arrival;
    int completionTriage; //indice del primo server che finisce
    int completionRossa;
    double current;
    double next;
    double last;
}event;

typedef struct multiserver{
    double service;
    int occupied;
}multiserver;

typedef enum color{
    white,
    green,
    yellow,
    red
}color;

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
        if(rossa[i].service < min)
            min = rossa[i].service;
    }
    return min;
}
double getArrival(){
    SelectStream(0);
    arrival+=Exponential(1/0.09);
    return arrival;
}

 int NextEvent(multiserver event[], int length)
/* ----------------------------------------------------------------
 * return the index of the next completion in the given multiserver
 * ----------------------------------------------------------------
 */
{
    int e = 0;                                      
    int i = 1;
    while (i < length) {        
        if ((event[i].service < event[e].service))
            e = i;
        i++; 
    }
     if(event[e].occupied==0){
        return -1;
    }
    return (e);
}


 int FindOne(multiserver event[])
/* ----------------------------------------------
 * return the index of the first available server
 * ----------------------------------------------
 */
{
    int s;
    int i = 0;
    while (event[i].occupied == 1)       /* find the index of the first available */
        i++;                             /* (idle) server                         */
    s = i;
    return (s);
}

color assignCode(){
    SelectStream(2);
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
double getServiceTriage(){
  SelectStream(1);
  return Exponential(24);                  //La media del servizio è di sei minuti, quindi ogni server ha media 4*6=24
}
double getServiceRossa(){
    SelectStream(3);
    return Exponential(225.5);
}
int main(){
    arrival=START;
    int counter = 0;
    int numeroCodaTriage=0;
    int numeroCodaRossa=0;
    PlantSeeds(SEED);
    event t;
    multiserver triage[SERVERSTRIAGE];      //inizializzare array per salvare il multiserver
    multiserver rossa[SERVERSRED];

    for(int i=0; i<SERVERSTRIAGE; i++){ 
        triage[i].occupied=0;
        triage[i].service=INF;
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
        if(t.completionRossa != -1)
            t.completionRossa = NextEvent(rossa, SERVERSRED);
        if(t.completionTriage != -1)
            t.completionTriage = NextEvent(triage, SERVERSTRIAGE);
        t.next=Min(t.arrival, triage, rossa);    //probabilmente non serve poi vediamo
        if(t.next==INF)
            break;
        t.current=t.next;
        if(t.current==t.arrival){
            numeroCodaTriage++;
            t.arrival=getArrival();
            if(t.arrival>STOP)
            {
                t.last=t.current;
                t.arrival=INF;
            }
            if(numeroCodaTriage<=SERVERSTRIAGE && numeroCodaTriage>0){
                int index = FindOne(triage); //trovare un server libero
                triage[index].service=t.current+getServiceTriage();
                triage[index].occupied=1;
                t.completionTriage=NextEvent(triage, SERVERSTRIAGE);
            }
        }
        else if(t.current==triage[t.completionTriage].service){
               numeroCodaTriage--;
               if(numeroCodaTriage-SERVERSTRIAGE>=0)
               {
                    triage[t.completionTriage].service=t.current+getServiceTriage();
                    triage[t.completionTriage].occupied=1;
                    
               }
               else{
                    triage[t.completionTriage].service=INF;
                    triage[t.completionTriage].occupied=0;
               }
               if(numeroCodaTriage>0)
                    t.completionTriage=NextEvent(triage, SERVERSTRIAGE);
                else
                    t.completionTriage=-1;
               color code = assignCode();
               switch (code)
               {
                case red:
                    numeroCodaRossa++;
                    counter++;
                    if(numeroCodaRossa<=SERVERSRED){
                        int index=FindOne(rossa);
                        rossa[index].service = t.current+getServiceRossa();
                        rossa[index].occupied = 1;
                        t.completionRossa = NextEvent(rossa, SERVERSRED);
                    }
                    /*Probabilità muoia qualcuno*/
                    else{
                        SelectStream(4);
                        double p = Uniform(0,100);
                        if(p<5.2){              //costante da decidere
                            numeroCodaRossa--;
                        }
                    }
                break;
                case yellow:
                    break;
               default:
                break;
               }
               }
               else if(t.current==rossa[t.completionRossa].service){
                    numeroCodaRossa--;
                    if(numeroCodaRossa-SERVERSRED>=0){
                        rossa[t.completionRossa].service=t.current+getServiceRossa();
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
            }
    return 0;
    }


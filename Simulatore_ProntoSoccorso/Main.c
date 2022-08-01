//
//  Main.c
//  Simulatore_ProntoSoccorso
//
//  Created by Matteo Federico on 12/05/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../DES_1e_SourceCode/rng.h"
#include "../../DES_1e_SourceCode/rngs.h"
#define START 0.0
#define STOP 2000.0
#define INF   (100.0 * STOP)
#define SEED 123456789

double arrival;

typedef struct event{
    double arrival;
    double completionTriage;

    double current;
    double next;
    double last;
}event;
   double Exponential(double m)
/* ---------------------------------------------------
 * generate an Exponential random variate, use m > 0.0 
 * ---------------------------------------------------
 */
{
  return (-m * log(1.0 - Random()));
}
double Min(double a, double c)
/* ------------------------------
 * return the smaller of a, b
 * ------------------------------
 */
{ 
  if (a < c)
    return (a);
  else
    return (c);
} 
double getArrival(){
    SelectStream(0);
    arrival+=Exponential(1.0/0.09);
    return arrival;
}
double getServiceTriage(){
  SelectStream(1);
  return Exponential(6.0);
}
int main(){
    arrival=START;
    int numeroCodaTriage=0;
    PlantSeeds(SEED);
    event t;
    t.arrival=getArrival();
    printf("primo arrivo: %f\n",t.arrival);
    t.completionTriage=INF;
    t.current=START;
    t.last=START;
    while(t.arrival<STOP || numeroCodaTriage>0)
    {
        printf("numero persone in coda %d\n",numeroCodaTriage);
        t.next=Min(t.arrival,t.completionTriage);
        t.current=t.next;
        if(t.current==t.arrival){
            numeroCodaTriage++;
            t.arrival=getArrival();
            printf("nuovo Arrivo: %f\n",t.arrival);
            if(t.arrival>STOP)
            {
                t.last=t.current;
                t.arrival=INF;
            }
            if(numeroCodaTriage==1){
                t.completionTriage=t.current+getServiceTriage();
            }
        }
        else{
            if(t.current==t.completionTriage){
               printf("completamento %f\n",t.completionTriage);
               numeroCodaTriage--;
               if(numeroCodaTriage>0)
               {
                    
                    t.completionTriage=getServiceTriage()+t.current;

               }
               else{
                t.completionTriage=INF;
               }
                /*
                    va assegnata il colore alla coda
                */
               }
               else{
                //eventi futuri dei completamenti degli altri reparti
               }        
        }
    }

    return 0;
}

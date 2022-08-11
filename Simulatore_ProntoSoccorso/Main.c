//
//  Main.c
//  Simulatore_ProntoSoccorso
//
//  Created by Matteo Federico, Adrian Petru Baba, Sara Da Canal on 12/05/22.
//
//


/*  USED STREAMS:
        0-4:    for each service completion random generation
        5:      for triage arrival time generation  
        6:      to generate the code assigned to smeone after he passes the triage
        7:      to generate the variable that let us decide if someone in red code dies or not
        8:      to decide wich node will take someone after he gets the code color yellow
        9:      to decide wich node will take someone after he gets the code color green
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libreriaRand/rngs.h" //spostare le librerie

#define START 0.0
#define STOP 500000.0
#define INF   (100.0 * STOP)
#define SEED 123456789

//size in # of servers, for each node
#define SERVERSTRIAGE 2
#define SERVERSRED 2
#define SERVERSTRAUMA 3    
#define SERVERSMEDICAL 4    
#define SERVERSMINOR 7   

//simulation variables
double arrival; 

//metadata needed to calculate output variables
//@name change
typedef struct nodeData{
    double node;                    /* time integrated number in the node  */
    double queue;                   /* time integrated number in the queue */
    double service;                 /* time integrated number in service   */
    int index;                      /* # of persons who passed the node */
    double current;
    int serverNumber;
} nodeData;

//event metadata
typedef struct event{     
    double arrival;

    //index of the next server of that specific node (ex. triage, red code) that wil have a completion
    int triageCompletion; 
    int redCodeCompletion;
    int traumaCompletion;
    int medicalCompletion;
    int minorCompletion;

    double current;
    double next;
    double last;
} event;

//code colors
typedef enum color{
    white,
    green,
    yellow,
    red,
    none
} color;

//multiserver metadata
typedef struct multiserver{
    double service;
    int occupied;
    color color;
} multiserver;

//data needed for each node of the system
typedef struct paramService{
    double mean;
    int stream;
}paramService;

paramService traumaParams;      
paramService medicalParams;
paramService minorParams;
paramService triageParams;
paramService redParams;

//initialization of the data needed to generate the service time for each node
void intitParamService(){
    traumaParams.mean=93.4;
    medicalParams.mean=165.9;
    minorParams.mean=105.6;
    redParams.mean=225.5;
    triageParams.mean=10.0;

    triageParams.stream=0;
    redParams.stream=1;
    traumaParams.stream=2;
    medicalParams.stream=3;
    minorParams.stream=4;
}

double Exponential(double m){
/* ---------------------------------------------------
 * generate an Exponential random variate, use m > 0.0 
 * ---------------------------------------------------
 */
    return (-m * log(1.0 - Random()));
}

double Uniform(double a, double b){
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */

    return (a + (b - a) * Random());
}

//@ added and replaced function at main start 
double getSpecificMin(double currentMin, multiserver *mserver, int serverDim){
 /*    ------------------------------------------------
 * return the smaller of between the current minimum and every multiserver service time
 * ------------------------------------------------
 */
    double min=currentMin;
    for(int i=0; i<serverDim; i++){
        if(mserver[i].service<min)  min = mserver[i].service;
    }
    return min;
}


double getArrival(){
/* ----------------------------
 * return the next arrival time
 * ----------------------------
 */
    SelectStream(5);
    arrival+=Exponential(1/0.09);
    return arrival;
}

int NextEvent(multiserver event[], int length){
/* ---------------------------------------
 * return the index of the next event type
 * ---------------------------------------
 */
    int e = 0;                                      
    int i = 1;
    while (i < length) { 
        if ((event[i].occupied == 1) && (event[i].service < event[e].service))
            e = i;
        i++; 
    }
    if(event[e].occupied==0){
        return -1;
    }
    return (e);
}

//@improved
int FindOne(multiserver mserver[]){
/* ----------------------------------------------
 * return the index of the first available server
 * ----------------------------------------------
 */
    int index = 0;
    while (mserver[index].occupied == 1)      
        index++;                      
    return (index);
}

color assignCode(){
/* -----------------------------------
 * return the code assigned to the job
 * -----------------------------------
 */
    SelectStream(6);
    double x = Uniform(0,100);
    if (x<1.04)
        return red;
    else if (x<19.44)
        return yellow;
    else if (x<80.15)
        return green;
    else
        return white;
}

double getService(paramService service){
/* --------------------------------------------------
 * return the next service time for the given service
 * --------------------------------------------------
 */
    SelectStream(service.stream);
    return Exponential(service.mean);
}

//init the noteData structure
void initOutputStats(nodeData *node, int dim)
{
    node->node=0.0;
    node->queue=0.0;
    node->service=0.0;
    node->index=0;
    node->serverNumber=dim;
}

//for debug only
void printStats(nodeData appoggio){
    printf("\nfor %d jobs\n", appoggio.index);
   // printf("   average interarrival time = %6.2f\n", t.last / appoggio.index);
    printf("   average wait ............ = %6.2f\n", appoggio.node / appoggio.index);
    printf("   average delay ........... = %6.2f\n", appoggio.queue / appoggio.index);
    printf("   average service time .... = %6.2f\n", appoggio.service / appoggio.index);
    printf("   average # in the node ... = %6.2f\n", appoggio.node / appoggio.current);
    printf("   average # in the queue .. = %6.2f\n", appoggio.queue / appoggio.current);
    printf("   statistiche.current= %6.2f\n   ",appoggio.current);
    printf("   utilization ............. = %6.2f\n", (appoggio.service/appoggio.serverNumber) / appoggio.current);
}

//stats updater for each step
void statsUpdater(nodeData *node, int num, double current, double next){
    node->node  += (next - current) * num;
    if(num>node->serverNumber)
        node->service += (next - current)*node->serverNumber;
    else
        node->service += (next - current)*num;
    node->current=current;
    if(num>node->serverNumber-1)
        node->queue   += (next - current) * (num - node->serverNumber);
}

//single queue stats updater
void partialStatsUpdater(nodeData *node, int num, int servers, double current, double next){
    node->node += (next - current) * num;
    node->service += (next - current)*servers;
    node->current=current;
    if(num>servers-1)
        node->queue += (next - current) * (num - servers);
}

//initialize each server metadata
void initServerData(multiserver *server, int dim){
    for(int i=0; i<dim; i++){
        server[i].occupied=0;
        server[i].service=INF;
        server[i].color=none;
    }
}

//@added!!
void modifyServerData(multiserver *server, double service, int occupied){
    server->service=service;
    server->occupied=occupied;
}

void modifyServerDataColor(multiserver *server, double service, int occupied, color color){
    modifyServerData(server, service, occupied);
    server->color=color;
}

int main(){

    nodeData triageStats;
    nodeData redCodeStats;
    nodeData traumaStats;
    nodeData medicalStats;
    nodeData minorStats;

    nodeData yellowTraumaStats;
    nodeData greenTraumaStats;

    nodeData yellowMedicalStats;
    nodeData greenMedicalStats;

    nodeData yellowMinorStats;
    nodeData greenMinorStats;
    nodeData whiteMinorStats;


    initOutputStats(&triageStats,SERVERSTRIAGE);
    initOutputStats(&redCodeStats,SERVERSRED);
    initOutputStats(&traumaStats,SERVERSTRAUMA);
    initOutputStats(&medicalStats,SERVERSMEDICAL);
    initOutputStats(&minorStats,SERVERSMINOR);
    //partial statistic
    initOutputStats(&yellowTraumaStats, SERVERSTRAUMA);
    initOutputStats(&greenTraumaStats, SERVERSTRAUMA);

    initOutputStats(&yellowMedicalStats, SERVERSMEDICAL);
    initOutputStats(&greenMedicalStats, SERVERSMEDICAL);

    initOutputStats(&yellowMinorStats, SERVERSMINOR);
    initOutputStats(&greenMinorStats, SERVERSMINOR);
    initOutputStats(&whiteMinorStats, SERVERSMINOR);

    intitParamService();

    arrival=START;

    //number of people in queue for a specific area (and code color)
    int triageNumber=0;
    int redNumber=0;

    int medicalYellowNumber=0;
    int medicalGreenNumber=0;

    int minorYellowNumber=0;
    int minorGreenNumber=0;
    int minorWhiteNumber=0;

    int traumaYellowNumber=0;
    int traumaGreenNumber=0;

    //sembra sensato tenere quanti ce ne sono in servizio per le statistiche
    int traumaInServiceYellow=0;
    int medicalInServiceYellow=0;
    int minorInServiceYellow=0;

    int traumaInServiceGreen=0;
    int medicalInServiceGreen=0;
    int minorInServiceGreen=0;

    int minorInServiceWhite=0;

    PlantSeeds(SEED);
    event t;

    //an array of servers for each multiserver
    multiserver triage[SERVERSTRIAGE];      
    multiserver redCode[SERVERSRED];
    multiserver trauma[SERVERSTRAUMA];   
    multiserver medical[SERVERSMEDICAL];
    multiserver minor[SERVERSMINOR];     

    initServerData(triage, SERVERSTRIAGE);
    initServerData(medical, SERVERSMEDICAL);
    initServerData(minor, SERVERSMINOR);
    initServerData(trauma, SERVERSTRAUMA);
    initServerData(redCode, SERVERSRED);

    t.arrival=getArrival();
    t.triageCompletion=-1;
    t.redCodeCompletion=-1;
    t.traumaCompletion=-1;
    t.medicalCompletion=-1;
    t.minorCompletion=-1;
    t.current=START;
    t.last=START;   //per statistiche non serve

    //@improved while condition
    while(t.arrival<STOP || 
    (triageNumber+redNumber+traumaYellowNumber+
    traumaGreenNumber+minorYellowNumber+minorGreenNumber+
    minorWhiteNumber+medicalYellowNumber+medicalGreenNumber)>0){
        t.next=getSpecificMin(t.arrival, triage, SERVERSTRIAGE);
        t.next=getSpecificMin(t.next, redCode, SERVERSRED);
        t.next=getSpecificMin(t.next, trauma, SERVERSTRAUMA); 
        t.next=getSpecificMin(t.next, medical, SERVERSMEDICAL); 
        t.next=getSpecificMin(t.next, minor, SERVERSMINOR);    
        if(triageNumber>0)  statsUpdater(&triageStats,triageNumber,t.current,t.next);
            
        if(redNumber>0)   statsUpdater(&redCodeStats,redNumber,t.current,t.next);

        if(traumaYellowNumber+traumaGreenNumber>0)  statsUpdater(&traumaStats,traumaYellowNumber+traumaGreenNumber,t.current,t.next);

        if(medicalYellowNumber+medicalGreenNumber>0)  statsUpdater(&medicalStats,medicalYellowNumber+medicalGreenNumber,t.current,t.next);

        if(minorYellowNumber+minorGreenNumber+minorWhiteNumber>0)  statsUpdater(&minorStats,minorYellowNumber+minorGreenNumber+minorWhiteNumber,t.current,t.next);

        if(traumaYellowNumber>0) partialStatsUpdater(&yellowTraumaStats, traumaYellowNumber, traumaInServiceYellow, t.current, t.next);

        if(traumaGreenNumber>0) partialStatsUpdater(&greenTraumaStats, traumaGreenNumber, traumaInServiceGreen, t.current, t.next);

        if(medicalYellowNumber>0) partialStatsUpdater(&yellowMedicalStats, medicalYellowNumber, medicalInServiceYellow, t.current, t.next);

        if(medicalGreenNumber>0) partialStatsUpdater(&greenMedicalStats, medicalGreenNumber, medicalInServiceGreen, t.current, t.next);
        
        if(minorYellowNumber>0) partialStatsUpdater(&yellowMinorStats,minorYellowNumber, minorInServiceYellow, t.current,t.next);
        
        if(minorGreenNumber>0) partialStatsUpdater(&greenMinorStats,minorGreenNumber,minorInServiceGreen,t.current,t.next);

        if(minorWhiteNumber>0) partialStatsUpdater(&whiteMinorStats,minorWhiteNumber,minorInServiceWhite,t.current,t.next);
    
        t.current=t.next;    //not needed?
        /* process an arrival */
        if(t.current==t.arrival){
            triageNumber++;
            triageStats.index++;

            t.arrival=getArrival();
            if(t.arrival>STOP){
                t.last=t.current;
                t.arrival=INF;
            }
            if(triageNumber<=SERVERSTRIAGE){
                int index = FindOne(triage); //find a free server
                triage[index].service=t.current+getService(triageParams);
                triage[index].occupied=1;
                t.triageCompletion=NextEvent(triage, SERVERSTRIAGE);
            }
        }

        /* completion of the triage phase and transfer to the next phase */
        else if(t.triageCompletion>-1 && t.current==triage[t.triageCompletion].service){
                triageNumber--;
                //@every server data modification replaced w function row 250
                if(triageNumber-SERVERSTRIAGE>=0)
                    modifyServerData(&triage[t.triageCompletion], t.current+getService(triageParams), 1);                 
                
                else
                    modifyServerData(&triage[t.triageCompletion], INF, 0);

                if(triageNumber > 0)
                    t.triageCompletion=NextEvent(triage, SERVERSTRIAGE);
                else
                    t.triageCompletion=-1;

                color code = assignCode();
                double p;    //p for probability stuff

                /* queue assignment, based on code color */
                switch (code){
                case red:
                    redNumber++;
                    redCodeStats.index++;
                    if(redNumber<=SERVERSRED){
                        int index=FindOne(redCode);
                        modifyServerData(&redCode[index], t.current+getService(redParams), 1); 
                        t.redCodeCompletion = NextEvent(redCode, SERVERSRED);
                    }

                    /*people in red code may die while waiting (if they don't get served immediately)... :( */
                    else{
                        SelectStream(7);
                        p = Uniform(0,100);
                        if(p<5.2){              
                            redNumber--;
                        }
                    }
                    break;

                case yellow:
                    SelectStream(8);    
                    p = Uniform(0,100);
                    if(p<26.7){              
                        traumaYellowNumber++;
                        traumaStats.index++;
                        yellowTraumaStats.index++;
                        if(traumaYellowNumber+traumaGreenNumber<=SERVERSTRAUMA){
                            traumaInServiceYellow++;
                            int index = FindOne(trauma); //find a free server
                            modifyServerDataColor(&trauma[index], t.current+getService(traumaParams), 1, yellow); 
                            t.traumaCompletion=NextEvent(trauma, SERVERSTRAUMA);
                        }
                    }
                    else if(p<51.4){         
                        medicalYellowNumber++;
                        medicalStats.index++;
                        yellowMedicalStats.index++;
                        if(medicalYellowNumber+medicalGreenNumber<=SERVERSMEDICAL){
                            medicalInServiceYellow++;
                            int index = FindOne(medical); 
                            modifyServerDataColor(&medical[index], t.current+getService(medicalParams), 1, yellow); 
                            t.medicalCompletion=NextEvent(medical, SERVERSMEDICAL);
                        }
                    }
                    else{    
                        minorYellowNumber++; 
                        minorStats.index++;
                        yellowMinorStats.index++;
                        if(minorYellowNumber+minorGreenNumber+minorWhiteNumber<=SERVERSMINOR){
                            minorInServiceYellow++;
                            int index = FindOne(minor); //find a free server
                            modifyServerDataColor(&minor[index], t.current+getService(minorParams), 1, yellow); 
                            t.minorCompletion=NextEvent(minor, SERVERSMINOR);
                        }
                    }
                    break;

                case green:
                    SelectStream(9);
                    p = Uniform(0,100);
                    if(p<26.7){             
                        traumaGreenNumber++;
                        traumaStats.index++;
                        greenTraumaStats.index++;
                        if(traumaYellowNumber+traumaGreenNumber<=SERVERSTRAUMA){
                            if(traumaYellowNumber-traumaInServiceYellow==0){
                                traumaInServiceGreen++;
                                int index = FindOne(trauma); 
                                modifyServerDataColor(&trauma[index], t.current+getService(traumaParams), 1, green);
                                t.traumaCompletion=NextEvent(trauma, SERVERSTRAUMA);
                            }
                        }
                    }
                    else if(p<51.4){        
                        medicalGreenNumber++;
                        medicalStats.index++;
                        greenMedicalStats.index++;
                        if(medicalYellowNumber+medicalGreenNumber<=SERVERSMEDICAL){
                            if(medicalYellowNumber-medicalInServiceYellow==0){
                                medicalInServiceGreen++;
                                int index = FindOne(medical); 
                                modifyServerDataColor(&medical[index], t.current+getService(medicalParams), 1, green);
                                t.medicalCompletion=NextEvent(medical, SERVERSMEDICAL);
                            }
                        }
                    }
                    else{    
                        minorGreenNumber++; 
                        minorStats.index++;
                        greenMinorStats.index++;
                        if(minorYellowNumber+minorGreenNumber+minorWhiteNumber<=SERVERSMINOR){
                            if(minorYellowNumber-minorInServiceYellow==0){
                                minorInServiceGreen++;
                                int index = FindOne(minor); //find a free server
                                modifyServerDataColor(&minor[index], t.current+getService(minorParams), 1, green);
                                t.minorCompletion=NextEvent(minor, SERVERSMINOR);
                            }
                        }
                    }
                    break;

                case white: 
                    minorWhiteNumber++;
                    minorStats.index++;
                    whiteMinorStats.index++;
                    if(minorYellowNumber+minorGreenNumber+minorWhiteNumber<=SERVERSMINOR){
                        if((minorYellowNumber-minorInServiceYellow==0) && (minorGreenNumber-minorInServiceGreen==0)){
                            minorInServiceWhite++;
                            int index = FindOne(minor); //find a free server
                            modifyServerDataColor(&minor[index], t.current+getService(minorParams), 1, white);
                            t.minorCompletion=NextEvent(minor, SERVERSMINOR);
                        }
                    }
                    break; 
                default:
                    break;
                }
            }

            /* trauma server completion */
            else if(t.traumaCompletion>-1 && t.current==trauma[t.traumaCompletion].service){
                if(trauma[t.traumaCompletion].color==yellow){
                    traumaYellowNumber--;
                    traumaInServiceYellow--;
                }
                else{
                    traumaGreenNumber--;
                    traumaInServiceGreen--;
                }
                //yellow codes have > priority than green codes
                if(traumaYellowNumber-traumaInServiceYellow>0){
                    modifyServerDataColor(&trauma[t.traumaCompletion], t.current+getService(traumaParams), 1, yellow);
                    traumaInServiceYellow++;
                }
                else if(traumaGreenNumber>=SERVERSTRAUMA-traumaInServiceYellow){
                    modifyServerDataColor(&trauma[t.traumaCompletion], t.current+getService(traumaParams), 1, green);
                    traumaInServiceGreen++;
                }
                else{
                    modifyServerDataColor(&trauma[t.traumaCompletion], INF, 0, none);
                }

                if(traumaYellowNumber+traumaGreenNumber>0)
                    t.traumaCompletion = NextEvent(trauma, SERVERSTRAUMA);
                else
                    t.traumaCompletion=-1;
            }

            /* medical server completion */
            else if(t.medicalCompletion>-1 && t.current==medical[t.medicalCompletion].service){
                if(medical[t.medicalCompletion].color==yellow){
                    medicalYellowNumber--;
                    medicalInServiceYellow--;
                }
                else{
                    medicalGreenNumber--;
                    medicalInServiceGreen--;
                }
                if(medicalYellowNumber-medicalInServiceYellow>0){
                    modifyServerDataColor(&medical[t.medicalCompletion], t.current+getService(medicalParams), 1, yellow);
                    medicalInServiceYellow++;
                }
                else if(medicalGreenNumber>=SERVERSMEDICAL-medicalInServiceYellow){
                    modifyServerDataColor(&medical[t.medicalCompletion], t.current+getService(medicalParams), 1, green);
                    medicalInServiceGreen++;
                }
                else{
                    modifyServerDataColor(&medical[t.medicalCompletion], INF, 0, none);
                }

                if(medicalYellowNumber+medicalGreenNumber>0)
                    t.medicalCompletion = NextEvent(medical, SERVERSMEDICAL);
                else
                    t.medicalCompletion=-1;
            }

            /* minor problems server completion */
            else if(t.minorCompletion>-1 && t.current==minor[t.minorCompletion].service){
                if(minor[t.minorCompletion].color==yellow){
                    minorYellowNumber--;
                    minorInServiceYellow--;
                }
                else if(minor[t.minorCompletion].color==green){
                    minorGreenNumber--;
                    minorInServiceGreen--;
                }
                else{
                    minorWhiteNumber--;
                    minorInServiceWhite--;
                }
                if(minorYellowNumber-minorInServiceYellow>0){
                    modifyServerDataColor(&minor[t.minorCompletion], t.current+getService(minorParams), 1, yellow);
                    minorInServiceYellow++;
                }
                else if(minorGreenNumber-minorInServiceGreen>0){
                    modifyServerDataColor(&minor[t.minorCompletion], t.current+getService(minorParams), 1, green);
                    minorInServiceGreen++;
                }
                else if(minorWhiteNumber>=SERVERSMINOR-minorInServiceYellow-minorInServiceGreen){
                    modifyServerDataColor(&minor[t.minorCompletion], t.current+getService(minorParams), 1, white);
                    minorInServiceWhite++;
                }
                else{
                    modifyServerDataColor(&minor[t.minorCompletion], INF, 0, none);
                }

                if(minorYellowNumber+minorGreenNumber+minorWhiteNumber>0)
                    t.minorCompletion = NextEvent(minor, SERVERSMINOR);
                else
                    t.minorCompletion=-1;
            }

            /* red code queue completion */
            else if(t.redCodeCompletion>-1 && t.current==redCode[t.redCodeCompletion].service){
                redNumber--;
                if(redNumber-SERVERSRED>=0){
                    modifyServerData(&redCode[t.redCodeCompletion], t.current+getService(redParams), 1);
                }
                else{
                    modifyServerData(&redCode[t.redCodeCompletion], INF, 0);
                }
                if(redNumber > 0)
                    t.redCodeCompletion=NextEvent(redCode, SERVERSRED);
                else
                    t.redCodeCompletion=-1;
            }

            //debug only
            /*
            printf("TEMPO:%f\n", t.current);
            printf("Tempi: %f\t %f\t %f\t %f\t %f\t %f\t", t.arrival, triage[t.triageCompletion].service, redCode[t.redCodeCompletion].service, trauma[t.traumaCompletion].service, medical[t.medicalCompletion].service, minor[t.minorCompletion].service); 
            printf("Triage:\t");
            for(int i=0;i<SERVERSTRIAGE;i++){
                printf("%f\t", triage[i].service);
            }  
            printf("\nRossa:\t");
            for(int i=0;i<SERVERSRED;i++){
                printf("%f\t", redCode[i].service);
            }   
            printf("\nTraumi:\t");
            for(int i=0;i<SERVERSTRAUMA;i++){
                printf("%f\t", trauma[i].service);
            } 
            printf("\nMedico\t");
            for(int i=0;i<SERVERSMEDICAL;i++){
                printf("%f\t", medical[i].service);
            } 
            printf("\nMinori:\t");
            for(int i=0;i<SERVERSMINOR;i++){
                printf("%f\t", minor[i].service);
            } 
            printf("\n");
            printf("Rossi: %d\tTraumi: %d\tMedico: %d\tMinori: %d\t Triage: %d\n",redNumber, traumaYellowNumber+traumaGreenNumber, medicalYellowNumber+medicalGreenNumber, minorYellowNumber+minorGreenNumber+minorWhiteNumber, triageNumber);
            */

    }
    //debug
    printf("\n\n*** STATISTICHE GLOBALI ***\n");
    printf("Triage\n");
    printStats(triageStats);
    printf("Codici rossi\n");
    printStats(redCodeStats);
    printf("Traumi\n");
    printStats(traumaStats);
    printf("Problemi minori\n");
    printStats(minorStats);
    printf("Problemi medici\n");
    printStats(medicalStats);
    printf("\n\n*** STATISTICHE PARZIALI TRAUMI***\n");
    printStats(yellowTraumaStats);
    printStats(greenTraumaStats);
    printf("\n\n*** STATISTICHE PARZIALI PROBLEMI MINORI***\n");
    printStats(yellowMinorStats);
    printStats(greenMinorStats);
    printStats(whiteMinorStats);
    printf("\n\n*** STATISTICHE PARZIALI PROBLEMI MEDICI***\n");
    printStats(yellowMedicalStats);
    printStats(greenMedicalStats);
    return 0;
}


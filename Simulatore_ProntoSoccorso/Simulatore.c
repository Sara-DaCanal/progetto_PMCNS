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
#include "libreriaRand/rngs.h"
#include "librerieProgetto/struct.h"
#include "Simulatore.h"
#include "librerieProgetto/utils.h"


//size in # of servers, for each node
#define SERVERSTRIAGE 2
#define SERVERSRED 2
#define SERVERSTRAUMA 3    
#define SERVERSMEDICAL 4    
#define SERVERSMINOR 7   

//simulation variables
double arrival; 
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


double getArrival(){
/* ----------------------------
 * return the next arrival time
 * ----------------------------
 */
    SelectStream(5);
    arrival+=Exponential(1/0.09);
    return arrival;
}


//for debug only
void printStats(nodeData appoggio){
    printf("\nfor %d jobs\n", appoggio.index);
    printf("   average wait ............ = %6.2f\n", appoggio.node / appoggio.index);
    printf("   average delay ........... = %6.2f\n", appoggio.queue / appoggio.index);
    printf("   average service time .... = %6.2f\n", appoggio.service / appoggio.index);
    printf("   average # in the node ... = %6.2f\n", appoggio.node / appoggio.current);
    printf("   average # in the queue .. = %6.2f\n", appoggio.queue / appoggio.current);
    printf("   statistiche.current= %6.2f\n   ",appoggio.current);
    printf("   utilization ............. = %6.2f\n", (appoggio.service/appoggio.serverNumber) / appoggio.current);
}


int simulatore(output matrix[][12],int iteration, int finite){
    int currentJob=0;
    int currentBatch=0;

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

    //PlantSeeds(SEED);
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
    //t.last=START;   //per statistiche non serve TODO cancellare

    //@improved while condition
    while((finite && t.arrival<STOP) || (!finite && (currentBatch!=numBatch || currentJob!=numJobInBatch)) ||
    (finite && triageNumber+redNumber+traumaYellowNumber+
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
    
        t.current=t.next;
        
        if(!finite && numJobInBatch==currentJob){
            writeStats(matrix[currentBatch],triageStats,0);
            writeStats(matrix[currentBatch],redCodeStats,1);
            writeStats(matrix[currentBatch],traumaStats,2);
            writeStats(matrix[currentBatch],minorStats,3);
            writeStats(matrix[currentBatch],medicalStats,4);
            writeStats(matrix[currentBatch],yellowTraumaStats,5);
            writeStats(matrix[currentBatch],greenTraumaStats,6);
            writeStats(matrix[currentBatch],yellowMinorStats,7);
            writeStats(matrix[currentBatch],greenMinorStats,8);
            writeStats(matrix[currentBatch],whiteMinorStats,9);
            writeStats(matrix[currentBatch],yellowMedicalStats,10);
            writeStats(matrix[currentBatch],greenMedicalStats,11);



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
            currentBatch++;
            currentJob=0;
        }
        


        /* process an arrival */
        if(t.current==t.arrival){
            triageNumber++;
            triageStats.index++;

            t.arrival=getArrival();
            if(finite && t.arrival>STOP){
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
                            currentJob++;
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
                currentJob++;
                //if(!finite) printf("TRAUMA\n");
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
                currentJob++;
                //if(!finite) printf("MEDICAL\n");
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
                currentJob++;
                //if(!finite) printf("MINOR\n");
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
                currentJob++;
                //if(!finite) printf("RED\n");
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

    }
    //debug
    writeStats(matrix[iteration],triageStats,0);
    writeStats(matrix[iteration],redCodeStats,1);
    writeStats(matrix[iteration],traumaStats,2);
    writeStats(matrix[iteration],minorStats,3);
    writeStats(matrix[iteration],medicalStats,4);

    writeStats(matrix[iteration],yellowTraumaStats,5);
    writeStats(matrix[iteration],greenTraumaStats,6);
    writeStats(matrix[iteration],yellowMinorStats,7);
    writeStats(matrix[iteration],greenMinorStats,8);
    writeStats(matrix[iteration],whiteMinorStats,9);
    writeStats(matrix[iteration],yellowMedicalStats,10);
    writeStats(matrix[iteration],greenMedicalStats,11);
    return 0;
}


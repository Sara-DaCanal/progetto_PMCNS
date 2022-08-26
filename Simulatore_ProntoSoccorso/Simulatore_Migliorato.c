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
        6:      to generate the code assigned to someone after he passes the triage
        7:      to generate the variable that let us decide if someone in red code dies or not
        8:      to decide wich node will take someone after he gets the code color Orange
        9:      to decide wich node will take someone after he gets the code color green
        10:     to decide if an new arrival yellow is blue or orange
        11:     to decide wich node will take someone after he gets the code color Blue
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
void intitParamService1(){
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


double getArrival1(){
/* ----------------------------
 * return the next arrival time
 * ----------------------------
 */
    SelectStream(5);
    arrival+=Exponential(1/0.09);
    return arrival;
}


//for debug only
void printStats1(nodeData appoggio){
    printf("\nfor %d jobs\n", appoggio.index);
    printf("   average wait ............ = %6.2f\n", appoggio.node / appoggio.index);
    printf("   average delay ........... = %6.2f\n", appoggio.queue / appoggio.index);
    printf("   average service time .... = %6.2f\n", appoggio.service / appoggio.index);
    printf("   average # in the node ... = %6.2f\n", appoggio.node / appoggio.current);
    printf("   average # in the queue .. = %6.2f\n", appoggio.queue / appoggio.current);
    printf("   statistiche.current= %6.2f\n   ",appoggio.current);
    printf("   utilization ............. = %6.2f\n", (appoggio.service/appoggio.serverNumber) / appoggio.current);
}


int simulatore2(output matrix[][15],int iteration, int finite){
    int currentJob=0;
    int currentBatch=0;

    nodeData triageStats;
    nodeData redCodeStats;
    nodeData traumaStats;
    nodeData medicalStats;
    nodeData minorStats;

    nodeData orangeTraumaStats;
    nodeData blueTraumaStats;
    nodeData greenTraumaStats;

    nodeData orangeMedicalStats;
    nodeData blueMedicalStats;
    nodeData greenMedicalStats;

    nodeData orangeMinorStats;
    nodeData blueMinorStats;
    nodeData greenMinorStats;
    nodeData whiteMinorStats;


    initOutputStats(&triageStats,SERVERSTRIAGE);
    initOutputStats(&redCodeStats,SERVERSRED);
    initOutputStats(&traumaStats,SERVERSTRAUMA);
    initOutputStats(&medicalStats,SERVERSMEDICAL);
    initOutputStats(&minorStats,SERVERSMINOR);
    //partial statistic
    
    initOutputStats(&orangeTraumaStats, SERVERSTRAUMA);
    initOutputStats(&blueTraumaStats, SERVERSTRAUMA);
    initOutputStats(&greenTraumaStats, SERVERSTRAUMA);
    
    initOutputStats(&orangeMedicalStats, SERVERSMEDICAL);
    initOutputStats(&blueMedicalStats, SERVERSMEDICAL);
    initOutputStats(&greenMedicalStats, SERVERSMEDICAL);
    
    initOutputStats(&orangeMinorStats, SERVERSMINOR);
    initOutputStats(&blueMinorStats, SERVERSMINOR);
    initOutputStats(&greenMinorStats, SERVERSMINOR);
    initOutputStats(&whiteMinorStats, SERVERSMINOR);

    intitParamService1();

    arrival=START;

    //number of people in queue for a specific area (and code color)
    int triageNumber=0;
    int redNumber=0;

    int medicalOrangeNumber=0;
    int medicalBlueNumber=0;
    int medicalGreenNumber=0;

    int minorOrangeNumber=0;
    int minorBlueNumber=0;
    int minorGreenNumber=0;
    int minorWhiteNumber=0;

    int traumaOrangeNumber=0;
    int traumaBlueNumber=0;
    int traumaGreenNumber=0;

    //sembra sensato tenere quanti ce ne sono in servizio per le statistiche
    int traumaInServiceOrange=0;
    int medicalInServiceOrange=0;
    int minorInServiceOrange=0;

    int traumaInServiceBlue=0;
    int medicalInServiceBlue=0;
    int minorInServiceBlue=0;

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
    t.arrival=getArrival1();
    t.triageCompletion=-1;
    t.redCodeCompletion=-1;
    t.traumaCompletion=-1;
    t.medicalCompletion=-1;
    t.minorCompletion=-1;
    t.current=START;
    //t.last=START;   //per statistiche non serve TODO cancellare

    //@improved while condition
    while((finite && t.arrival<STOP) || (!finite && (currentBatch!=numBatch || currentJob!=numJobInBatch)) ||
    (finite && triageNumber+redNumber+traumaOrangeNumber+traumaBlueNumber+
    traumaGreenNumber+minorOrangeNumber+minorBlueNumber+minorGreenNumber+
    minorWhiteNumber+medicalOrangeNumber+medicalBlueNumber+medicalGreenNumber)>0){
        /*
        printf("%d\n",t.redCodeCompletion);
        printf("current:%f \t arrival:%f \t %d triageCompletion:%f \t %d redCodeCompletion:%f \t %d traumaCompletion:%f \t %d medicalCompletion:%f\t %d minorCompletion:%f\n",t.current,t.arrival,t.triageCompletion,triage[t.triageCompletion].service,t.redCodeCompletion,redCode[t.redCodeCompletion].service,t.traumaCompletion ,trauma[t.traumaCompletion].service,t.medicalCompletion,medical[t.medicalCompletion].service,t.minorCompletion,minor[t.minorCompletion].service);
        printf("Triage\n");
        for (int i = 0; i < SERVERSTRIAGE; ++i)
        {
            printf("%f\t",triage[i].service );

        }
        printf("\n");
        printf("trauma\n");
        for (int i = 0; i < SERVERSTRAUMA; i++)
        {
            printf("%f\t",trauma[i].service );

        }
        printf("\n");
        printf("medical\n");

        for (int i = 0; i < SERVERSMEDICAL; i++)
        {
            printf("%f\t",medical[i].service );

        }
        printf("\n");
        printf("minor\n");
        for (int i = 0; i < SERVERSMINOR; i++)
        {
            printf("%f\t",minor[i].service );

        }
        printf("\n");
        printf("red\n");
        for (int i = 0; i < SERVERSRED; i++)
        {
            printf("%f\t",redCode[i].service );

        }
        printf("\n");
        */




        t.next=getSpecificMin(t.arrival, triage, SERVERSTRIAGE);
        t.next=getSpecificMin(t.next, redCode, SERVERSRED);
        t.next=getSpecificMin(t.next, trauma, SERVERSTRAUMA); 
        t.next=getSpecificMin(t.next, medical, SERVERSMEDICAL); 
        t.next=getSpecificMin(t.next, minor, SERVERSMINOR);    
        if(triageNumber>0)  statsUpdater(&triageStats,triageNumber,t.current,t.next);
            
        if(redNumber>0)   statsUpdater(&redCodeStats,redNumber,t.current,t.next);

        if(traumaOrangeNumber+traumaBlueNumber+traumaGreenNumber>0)  statsUpdater(&traumaStats,traumaOrangeNumber+traumaBlueNumber+traumaGreenNumber,t.current,t.next);

        if(medicalOrangeNumber+medicalBlueNumber+medicalGreenNumber>0)  statsUpdater(&medicalStats,medicalOrangeNumber+medicalBlueNumber+medicalGreenNumber,t.current,t.next);

        if(minorOrangeNumber+minorBlueNumber+minorGreenNumber+minorWhiteNumber>0)  statsUpdater(&minorStats,minorOrangeNumber+minorBlueNumber+minorGreenNumber+minorWhiteNumber,t.current,t.next);

        if(traumaOrangeNumber>0) partialStatsUpdater(&orangeTraumaStats, traumaOrangeNumber, traumaInServiceOrange, t.current, t.next);

        if(traumaBlueNumber>0) partialStatsUpdater(&blueTraumaStats, traumaBlueNumber, traumaInServiceBlue, t.current, t.next);

        if(traumaGreenNumber>0) partialStatsUpdater(&greenTraumaStats, traumaGreenNumber, traumaInServiceGreen, t.current, t.next);

        if(medicalOrangeNumber>0) partialStatsUpdater(&orangeMedicalStats, medicalOrangeNumber, medicalInServiceOrange, t.current, t.next);

        if(medicalBlueNumber>0) partialStatsUpdater(&blueMedicalStats, medicalBlueNumber, medicalInServiceBlue, t.current, t.next);

        if(medicalGreenNumber>0) partialStatsUpdater(&greenMedicalStats, medicalGreenNumber, medicalInServiceGreen, t.current, t.next);
        
        if(minorOrangeNumber>0) partialStatsUpdater(&orangeMinorStats,minorOrangeNumber, minorInServiceOrange, t.current,t.next);

        if(minorBlueNumber>0) partialStatsUpdater(&blueMinorStats,minorBlueNumber, minorInServiceBlue, t.current,t.next);

        if(minorGreenNumber>0) partialStatsUpdater(&greenMinorStats,minorGreenNumber,minorInServiceGreen,t.current,t.next);

        if(minorWhiteNumber>0) partialStatsUpdater(&whiteMinorStats,minorWhiteNumber,minorInServiceWhite,t.current,t.next);
    
        t.current=t.next;
        
        if(!finite && numJobInBatch==currentJob){
            writeStats(matrix[currentBatch],triageStats,0);
            writeStats(matrix[currentBatch],redCodeStats,1);
            writeStats(matrix[currentBatch],traumaStats,2);
            writeStats(matrix[currentBatch],minorStats,3);
            writeStats(matrix[currentBatch],medicalStats,4);
            writeStats(matrix[currentBatch],orangeTraumaStats,5);
            writeStats(matrix[currentBatch],blueTraumaStats,6);
            writeStats(matrix[currentBatch],greenTraumaStats,7);
            writeStats(matrix[currentBatch],orangeMinorStats,8);
            writeStats(matrix[currentBatch],blueMinorStats,9);
            writeStats(matrix[currentBatch],greenMinorStats,10);
            writeStats(matrix[currentBatch],whiteMinorStats,11);
            writeStats(matrix[currentBatch],orangeMedicalStats,12);
            writeStats(matrix[currentBatch],blueMedicalStats,13);
            writeStats(matrix[currentBatch],greenMedicalStats,14);



            initOutputStats(&triageStats,SERVERSTRIAGE);
            initTime(&triageStats,t.current);
            initOutputStats(&redCodeStats,SERVERSRED);
            initTime(&redCodeStats,t.current);
            initOutputStats(&traumaStats,SERVERSTRAUMA);
            initTime(&traumaStats,t.current);
            initOutputStats(&medicalStats,SERVERSMEDICAL);
            initTime(&medicalStats,t.current);
            initOutputStats(&minorStats,SERVERSMINOR);
            initTime(&minorStats,t.current);
            
            //partial statistic
            initOutputStats(&orangeTraumaStats, SERVERSTRAUMA);
            initTime(&orangeTraumaStats,t.current);
            
            initOutputStats(&blueTraumaStats, SERVERSTRAUMA);
            initTime(&blueTraumaStats,t.current);
            
            initOutputStats(&greenTraumaStats, SERVERSTRAUMA);
            initTime(&greenTraumaStats,t.current);

            initOutputStats(&orangeMedicalStats, SERVERSMEDICAL);
            initTime(&orangeMedicalStats,t.current);

            initOutputStats(&blueMedicalStats, SERVERSMEDICAL);
            initTime(&blueMedicalStats,t.current);
            
            initOutputStats(&greenMedicalStats, SERVERSMEDICAL);
            initTime(&greenMedicalStats,t.current);

            
            initOutputStats(&orangeMinorStats, SERVERSMINOR);
            initTime(&orangeMinorStats,t.current);

            initOutputStats(&blueMinorStats, SERVERSMINOR);
            initTime(&blueMinorStats,t.current);

            initOutputStats(&greenMinorStats, SERVERSMINOR);
            initTime(&greenMinorStats,t.current);
            
            initOutputStats(&whiteMinorStats, SERVERSMINOR);
            initTime(&whiteMinorStats,t.current);
            
            currentBatch++;
            currentJob=0;
        }
        


        /* process an arrival */
        if(t.current==t.arrival){
            triageNumber++;
            triageStats.index++;

            t.arrival=getArrival1();
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
                    SelectStream(10);
                    p=Uniform(0,100);
                    if(p<0.5) //TO-DO decidere la probabilità di essere arancioni o blu
                    {
                        //orange
                        SelectStream(8);    
                        p = Uniform(0,100);
                        if(p<26.7){              
                            traumaOrangeNumber++;
                            traumaStats.index++;
                            orangeTraumaStats.index++;
                            if(traumaOrangeNumber+traumaBlueNumber+traumaGreenNumber<=SERVERSTRAUMA){
                                traumaInServiceOrange++;
                                int index = FindOne(trauma); //find a free server
                                modifyServerDataColor(&trauma[index], t.current+getService(traumaParams), 1, orange); 
                                t.traumaCompletion=NextEvent(trauma, SERVERSTRAUMA);
                            }
                        }
                        else if(p<51.4){         
                            medicalOrangeNumber++;
                            medicalStats.index++;
                            orangeMedicalStats.index++;
                            if(medicalOrangeNumber+medicalBlueNumber+medicalGreenNumber<=SERVERSMEDICAL){
                                medicalInServiceOrange++;
                                int index = FindOne(medical); 
                                modifyServerDataColor(&medical[index], t.current+getService(medicalParams), 1, orange); 
                                t.medicalCompletion=NextEvent(medical, SERVERSMEDICAL);
                            }
                        }
                        else{    
                            minorOrangeNumber++; 
                            minorStats.index++;
                            orangeMinorStats.index++;
                            if(minorOrangeNumber+minorBlueNumber+minorGreenNumber+minorWhiteNumber<=SERVERSMINOR){
                                minorInServiceOrange++;
                                int index = FindOne(minor); //find a free server
                                modifyServerDataColor(&minor[index], t.current+getService(minorParams), 1, orange); 
                                t.minorCompletion=NextEvent(minor, SERVERSMINOR);
                            }
                        }
                    }
                    else{
                        //blue
                        SelectStream(11); 
                        p = Uniform(0,100);
                        if(p<26.7){          
                            traumaBlueNumber++;
                            traumaStats.index++;
                            blueTraumaStats.index++;
                            if(traumaOrangeNumber+traumaBlueNumber+traumaGreenNumber<=SERVERSTRAUMA){
                                if(traumaOrangeNumber-traumaInServiceOrange==0){
                                    traumaInServiceBlue++;
                                    int index = FindOne(trauma); //find a free server
                                    modifyServerDataColor(&trauma[index], t.current+getService(traumaParams), 1, blue); 
                                    t.traumaCompletion=NextEvent(trauma, SERVERSTRAUMA);
                                }
                            }
                        }
                        else if(p<51.4){   
                            medicalBlueNumber++;
                            medicalStats.index++;
                            blueMedicalStats.index++;
                            if(medicalOrangeNumber+medicalBlueNumber+medicalGreenNumber<=SERVERSMEDICAL){
                                if(medicalOrangeNumber-medicalInServiceOrange==0){
                                    medicalInServiceBlue++;
                                    int index = FindOne(medical); 
                                    modifyServerDataColor(&medical[index], t.current+getService(medicalParams), 1, blue); 
                                    t.medicalCompletion=NextEvent(medical, SERVERSMEDICAL);
                                }
                            }
                        }
                        else{     
                            minorBlueNumber++; 
                            minorStats.index++;
                            blueMinorStats.index++;
                            if(minorOrangeNumber+minorBlueNumber+minorGreenNumber+minorWhiteNumber<=SERVERSMINOR){
                                if(minorOrangeNumber-minorInServiceOrange==0){
                                    minorInServiceBlue++;
                                    int index = FindOne(minor); //find a free server
                                    modifyServerDataColor(&minor[index], t.current+getService(minorParams), 1, blue); 
                                    t.minorCompletion=NextEvent(minor, SERVERSMINOR);
                                }
                            }
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
                        if(traumaOrangeNumber+traumaBlueNumber+traumaGreenNumber<=SERVERSTRAUMA){
                            if(traumaOrangeNumber-traumaInServiceOrange==0 && traumaBlueNumber-traumaInServiceBlue==0){
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
                        if(medicalOrangeNumber+medicalBlueNumber+medicalGreenNumber<=SERVERSMEDICAL){
                            if(medicalOrangeNumber-medicalInServiceOrange==0 && medicalBlueNumber-medicalInServiceBlue==0){
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
                        if(minorOrangeNumber+minorBlueNumber+minorGreenNumber+minorWhiteNumber<=SERVERSMINOR){
                            if(minorOrangeNumber-minorInServiceOrange==0 && minorBlueNumber-minorInServiceBlue==0){
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
                    if(minorOrangeNumber+minorBlueNumber+minorGreenNumber+minorWhiteNumber<=SERVERSMINOR){
                        if((minorOrangeNumber-minorInServiceOrange==0 && minorBlueNumber-minorInServiceBlue==0) && (minorGreenNumber-minorInServiceGreen==0)){
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
                if(trauma[t.traumaCompletion].color==orange){
                    traumaOrangeNumber--;
                    traumaInServiceOrange--;
                }
                else if(trauma[t.traumaCompletion].color==blue){
                    traumaBlueNumber--;
                    traumaInServiceBlue--;
                }
                else{
                    traumaGreenNumber--;
                    traumaInServiceGreen--;
                }

                //orange codes have > priority than blue and green codes
                if(traumaOrangeNumber-traumaInServiceOrange>0){
                    modifyServerDataColor(&trauma[t.traumaCompletion], t.current+getService(traumaParams), 1, orange);
                    traumaInServiceOrange++;
                }
                else if(traumaBlueNumber-traumaInServiceBlue>0){
                    modifyServerDataColor(&trauma[t.traumaCompletion], t.current+getService(traumaParams), 1, blue);
                    traumaInServiceBlue++;
                }
                else if(traumaGreenNumber-traumaInServiceGreen>0){
                    modifyServerDataColor(&trauma[t.traumaCompletion], t.current+getService(traumaParams), 1, green);
                    traumaInServiceGreen++;
                }
                else{
                    modifyServerDataColor(&trauma[t.traumaCompletion], INF, 0, none);
                }

                if(traumaOrangeNumber+traumaBlueNumber+traumaGreenNumber>0)
                    t.traumaCompletion = NextEvent(trauma, SERVERSTRAUMA);
                else
                    t.traumaCompletion=-1;
            }

            /* medical server completion */
            else if(t.medicalCompletion>-1 && t.current==medical[t.medicalCompletion].service){
                currentJob++;
                if(medical[t.medicalCompletion].color==orange){
                    medicalOrangeNumber--;
                    medicalInServiceOrange--;
                }
                else if(medical[t.medicalCompletion].color==blue){
                    medicalBlueNumber--;
                    medicalInServiceBlue--;
                }
                else{
                    medicalGreenNumber--;
                    medicalInServiceGreen--;
                }

                if(medicalOrangeNumber-medicalInServiceOrange>0){
                    modifyServerDataColor(&medical[t.medicalCompletion], t.current+getService(medicalParams), 1, orange);
                    medicalInServiceOrange++;
                }
                else if(medicalBlueNumber-medicalInServiceBlue>0){
                    modifyServerDataColor(&medical[t.medicalCompletion], t.current+getService(medicalParams), 1, blue);
                    medicalInServiceBlue++;
                }
                else if(medicalGreenNumber-medicalInServiceGreen>0){
                    modifyServerDataColor(&medical[t.medicalCompletion], t.current+getService(medicalParams), 1, green);
                    medicalInServiceGreen++;
                }
                else{
                    modifyServerDataColor(&medical[t.medicalCompletion], INF, 0, none);
                }
                if(medicalOrangeNumber+medicalBlueNumber+medicalGreenNumber>0)
                    t.medicalCompletion = NextEvent(medical, SERVERSMEDICAL);
                else
                    t.medicalCompletion=-1;
            }

            /* minor problems server completion */
            else if(t.minorCompletion>-1 && t.current==minor[t.minorCompletion].service){
                currentJob++;
                //if(!finite) printf("MINOR\n");
                if(minor[t.minorCompletion].color==orange){
                    minorOrangeNumber--;
                    minorInServiceOrange--;
                }
                else if(minor[t.minorCompletion].color==blue){
                    minorBlueNumber--;
                    minorInServiceBlue--;
                }
                else if(minor[t.minorCompletion].color==green){
                    minorGreenNumber--;
                    minorInServiceGreen--;
                }
                else{
                    minorWhiteNumber--;
                    minorInServiceWhite--;
                }
                if(minorOrangeNumber-minorInServiceOrange>0){
                    modifyServerDataColor(&minor[t.minorCompletion], t.current+getService(minorParams), 1, orange);
                    minorInServiceOrange++;
                }
                else if(minorBlueNumber-minorInServiceBlue>0){
                    modifyServerDataColor(&minor[t.minorCompletion], t.current+getService(minorParams), 1, blue);
                    minorInServiceBlue++;
                }
                else if(minorGreenNumber-minorInServiceGreen>0){
                    modifyServerDataColor(&minor[t.minorCompletion], t.current+getService(minorParams), 1, green);
                    minorInServiceGreen++;
                }
                else if(minorWhiteNumber- minorInServiceWhite>0){
                    modifyServerDataColor(&minor[t.minorCompletion], t.current+getService(minorParams), 1, white);
                    minorInServiceWhite++;
                }
                else{
                    modifyServerDataColor(&minor[t.minorCompletion], INF, 0, none);
                }

                if(minorOrangeNumber+minorBlueNumber+minorGreenNumber+minorWhiteNumber>0)
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
      //  getchar();

    }
    //debug
    writeStats(matrix[currentBatch],triageStats,0);
    writeStats(matrix[currentBatch],redCodeStats,1);
    writeStats(matrix[currentBatch],traumaStats,2);
    writeStats(matrix[currentBatch],minorStats,3);
    writeStats(matrix[currentBatch],medicalStats,4);
    writeStats(matrix[currentBatch],orangeTraumaStats,5);
    writeStats(matrix[currentBatch],blueTraumaStats,6);
    writeStats(matrix[currentBatch],greenTraumaStats,7);
    writeStats(matrix[currentBatch],orangeMinorStats,8);
    writeStats(matrix[currentBatch],blueMinorStats,9);
    writeStats(matrix[currentBatch],greenMinorStats,10);
    writeStats(matrix[currentBatch],whiteMinorStats,11);
    writeStats(matrix[currentBatch],orangeMedicalStats,12);
    writeStats(matrix[currentBatch],blueMedicalStats,13);
    writeStats(matrix[currentBatch],greenMedicalStats,14);

    return 0;
}

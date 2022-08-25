#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../libreriaRand/rngs.h"
#include "struct.h"
#include "utils.h"

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

double getService(paramService service){
/* --------------------------------------------------
 * return the next service time for the given service
 * --------------------------------------------------
 */
    SelectStream(service.stream);
    return Exponential(service.mean);
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
void initOutputStats(nodeData *node, int dim)
{
    node->node=0.0;
    node->queue=0.0;
    node->service=0.0;
    node->index=0;
    node->serverNumber=dim;
}

void writeStats(output out[],nodeData appoggio,int i){
    out[i].job+=appoggio.index;
    out[i].wait+=(appoggio.node / appoggio.index);
    out[i].delay+=(appoggio.queue / appoggio.index);
    out[i].service+=(appoggio.service / appoggio.index);
    out[i].numberNode+=(appoggio.node / appoggio.current);
    out[i].numberQueue+=(appoggio.queue / appoggio.current);
    out[i].utilization+=((appoggio.service/appoggio.serverNumber) / appoggio.current);
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

void modifyServerData(multiserver *server, double service, int occupied){
    server->service=service;
    server->occupied=occupied;
}

void modifyServerDataColor(multiserver *server, double service, int occupied, color color){
    modifyServerData(server, service, occupied);
    server->color=color;
}
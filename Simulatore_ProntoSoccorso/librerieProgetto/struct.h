/* contiene le struct e gli enumeratori del progetto */

//metadata needed to calculate output variables
//@name change
#ifndef struct_h
#define struct_h 

typedef struct nodeData{
    double node;                    /* time integrated number in the node  */
    double queue;                   /* time integrated number in the queue */
    double service;                 /* time integrated number in service   */
    int index;                      /* # of persons who passed the node */
    double current;
    int serverNumber;
} nodeData;

typedef struct output{
    double wait;
    double delay;
    double service;
    double numberNode;
    double numberQueue;
    double utilization;
    int job;
}output;

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

#endif
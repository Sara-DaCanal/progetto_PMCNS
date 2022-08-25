#ifndef utils_h
#define utils_h

#define START 0.0
#define STOP 1440.0
#define INF pow(1.79769, 308.0)
#define numBatch 64
#define numJobInBatch 1024

double Exponential(double m);
double Uniform(double a, double b);
double getSpecificMin(double currentMin, multiserver *mserver, int serverDim);
int NextEvent(multiserver event[], int length);
int FindOne(multiserver mserver[]);
double getService(paramService service);
color assignCode();
void initOutputStats(nodeData *node, int dim);
void writeStats(output out[],nodeData appoggio,int i);
void statsUpdater(nodeData *node, int num, double current, double next);
void partialStatsUpdater(nodeData *node, int num, int servers, double current, double next);
void initServerData(multiserver *server, int dim);
void modifyServerData(multiserver *server, double service, int occupied);
void modifyServerDataColor(multiserver *server, double service, int occupied, color color);

#endif
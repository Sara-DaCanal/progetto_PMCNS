//
//  Simulatore.h
//  Simulatore_ProntoSoccorso
//
//

#ifndef Simulatore_h
#define Simulatore_h

#include <stdio.h>
#define SEED 123456789

#define SERVERSTRIAGE 2 // 2 or 3
#define SERVERSRED 2
#define SERVERSTRAUMA 3   
#define SERVERSMEDICAL 4   
#define SERVERSMINOR 6

int simulatore(output matrix[][15], double decessi[], int iteration, int finite, float probability);
int simulatore2(output matrix[][15], double decessi[], int iteration, int finite,float probability);

#endif
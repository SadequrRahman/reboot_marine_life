/*
 * File:   SensorNode.h
 * Author: Mohammad Sadequr Rahman
 *
 *
 */
#include "NodeSimulator.h"

#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */


void nodeSimulator_init(void){
	/* initialize random seed: */
 	srand (time(NULL));
}

sensor_data_t nodeSimulator_getDataPoint(void){
	
	sensor_data_t data;

	data.ph = (rand() % 10 ) / 10 + ( rand() % 5);
	data.salinity = (rand() % 10 ) / 10 + ( rand() % 100);
	data._do = (rand() % 10 ) / 10 + ( rand() % 50);
	data._orp = (rand() % 10 ) / 10 + ( rand() % 60);
	data._hydroponics = (rand() % 10 ) / 10 + ( rand() % 500);

	return data;

}
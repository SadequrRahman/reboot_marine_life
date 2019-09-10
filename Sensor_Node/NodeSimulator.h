/*
 * File:   SensorNode.h
 * Author: Mohammad Sadequr Rahman
 *
 *
 */

#ifndef __NODESIMULATOR_H__
#define __NODESIMULATOR_H__


/* data model for representing sensor data */
typedef struct{
	float ph;
	float salinity;
	float _do;
	float _orp;
	float _hydroponics;
}sensor_data_t;


void nodeSimulator_init(void);
sensor_data_t nodeSimulator_getDataPoint(void);


#endif /* __NODESIMULATOR_H__ */
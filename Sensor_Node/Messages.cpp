/*
 * File:   Messages.cpp
 * Author: Mohammad Sadequr Rahman
 *	
 */
#include "Messages.h"
#include "Config.h"



/*
 * messages_generate
 * 
 */
Json::Value messages_generate(sensor_data_t data){

	Json::Value root; 

	root["Device"] = DEVICE_NAME;
	root["PH"] = data.ph;
	root["salinity"] = data.salinity;
	root["do"] = data._do; 
	root["orp"] = data._orp;
	root["_hydroponics"] = data._hydroponics;

	return root;
	
}

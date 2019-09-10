/*
	File : main.c
	Author : mohammad sadequr Rahman 
	Date : 10/09/2019

	Desription: This is intended to simulate a sensor node
				for UNTIL project. It will generate stimulus
				and send this data point to server.

*/

#include <iostream>
#include <wiringPi.h>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include "Logger.h"
#include "Messages.h"
#include "NodeSimulator.h"




using namespace std;

int main(int argc, char const *argv[]){
	
	wiringPiSetupSys();
	nodeSimulator_init();
	Logger::getLogger();
	delay(1000);
	Json::FastWriter fastwriter;
	std::string message;

	while(true){

		Json::Value msg = messages_generate(nodeSimulator_getDataPoint());
		
	  	message = fastwriter.write(msg);
	  	std::cout<<message<<std::endl;

		Logger::getLogger() << message << "\r\n";
		delay(2000);
	}

	return 0;
}
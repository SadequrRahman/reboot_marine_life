/*
* File:   Logger.cpp
* Author: Mohammad Sadequr Rahman
*	
* 
* 
* 
*/
#include "Logger.h"
#include "Config.h"
#include <iostream>
#include <wiringPi.h>

Logger* Logger::logger = nullptr;

Logger::Logger(server_config_t config) : ServerBase(config)
{

}

Logger & Logger::getLogger(void)
{
	if (logger == nullptr) {
		server_config_t config = {
			 LOG_SERVER_PORT,
			 LOG_SERVER_BACKLOG,
			 LOG_SERVER_IPV,
			 LOG_SERVER_SOCKET_TYPE,
			 LOG_SERVER_IS_ALIVE,
			 LOG_SERVER_IS_REUSABLE
		};
		logger = new Logger(config);
		logger->Start();
	}
	return (*logger);
}

Logger::~Logger()
{
}

void Logger::handle_rx()
{
	bool over = false;
	char recMsg;
	client_info_t c = getClient();
	while (!over)
	{
		int recLen = read(c.accepted_sock_id, &recMsg, 1);
		if (recLen <= 0) {
			over = true;
			notifyDisconnect();
			break;
		}
	}

}

void Logger::handle_tx()
{
	bool over = false;
	client_info_t c = getClient();
	int size = 0;
	ssMutex.Lock();
	ss << "Device Type: " << DEVICE_NAME << "\r\nVersion Number: " << VERION_MAJOR << "." << VERION_MINOR << "\r\n";
	ssMutex.Unlock();
	while (!over)
	{
		ssMutex.Lock();
		size = ss.str().length();
		if (size > 0)
		{
			if (send(c.accepted_sock_id, ss.str().c_str(), size , 0) == -1)
			{
				over = true;
				notifyDisconnect();
				break;
			}
			ss.str("");
		}
		ssMutex.Unlock();
		delay(100);
	}

}

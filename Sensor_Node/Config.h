/*
 * File:   Config.h
 * Author: Mohammad Sadequr Rahman
 *	
 * This file holds the configuration of the 
	 * project and also the device type as well as 
	 * version number of the application.
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__

#define DEVICE_NAME				"Node 001"
#define VERION_MAJOR			0
#define VERION_MINOR			1


#define LOG_SERVER_PORT			2000
#define LOG_SERVER_BACKLOG		1
#define LOG_SERVER_IPV			IPv4
#define LOG_SERVER_SOCKET_TYPE	TCP
#define LOG_SERVER_IS_ALIVE		1
#define LOG_SERVER_IS_REUSABLE	1


#define RX_BUFFER_SIZE			4096
#define TX_BUFFER_SIZE			4096




#endif // !__CONFIG_H__

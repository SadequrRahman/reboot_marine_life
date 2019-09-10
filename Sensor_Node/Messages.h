/*
 * File:   Messages.h
 * Author: Mohammad Sadequr Rahman
 *	
 *	Implementation of our custom protocol
 *  This file contain the msg command definetion and'
 *  structure of a messages. It also provides a method 
 *  to parse a message from a incomming buffer.
 */
#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <stdint.h>
#include <jsoncpp/json/json.h>
#include "NodeSimulator.h"


Json::Value messages_generate(sensor_data_t data);

#endif //__MESSAGES_H__


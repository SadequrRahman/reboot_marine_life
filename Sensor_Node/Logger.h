/*
* File:   Logger.cpp
* Author: Mohammad Sadequr Rahman
*	
* 
* 
* 
*/
#ifndef __LOGGER_H__
#define __LOGGER_H__
 
#include "ServerBase.h"
#include <sstream>
#include <string>

using namespace std;

class Logger : public ServerBase
{
public:
	static Logger& getLogger(void);
	virtual ~Logger();
	void handle_rx() override;
	void handle_tx() override;
	template <typename T>
	Logger& operator<<(T str);
	
private:
	Logger(server_config_t config);
	static Logger *logger;
	stringstream ss;
	Mutex ssMutex;

};

template<typename T>
Logger& Logger::operator<<(T m)
{
	if (isClientConnected()) {
		ssMutex.Lock();
		ss << m;
		ssMutex.Unlock();
	}
	return (*this);
}

#endif //!__LOGGER_H__
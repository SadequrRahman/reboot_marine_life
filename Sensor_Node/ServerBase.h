/*
 * File:   ServerBase.h
 * Author: Mohammad Sadequr Rahman
 * 
 *	This a base class for Server. It has to be extended 
 *  for receiver and transmitter Server application.
 *  It provides two handler (such as handle_rx and handle_tx)
 *  as a seperated thread for accepted sock functionality.
 *  
 */
#ifndef __SERVERBASE_H__
#define __SERVERBASE_H__

#include <vector>
#include <string>
#include "Thread.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

typedef struct
{
	uint32_t port;
	uint8_t backlog;
	uint8_t ipv;
	uint8_t socketType;
	int iskeepAlive;
	int isReusable;
}server_config_t;

typedef struct {
	bool is_a_client;
	uint8_t cState;
	pthread_t rxt_id;
	pthread_t txt_id;
	int accepted_sock_id;
	struct sockaddr_storage client;
}client_info_t;

enum {
	IPv4,
	IPv6,
	UNSP
};

enum {
	TCP,
	UDP
};

class ServerBase : public Thread
{
public:

	ServerBase(server_config_t& _config);
	virtual ~ServerBase();
	virtual void handle_rx(void) = 0;
	virtual void handle_tx(void) = 0;
	void notifyDisconnect(void);
	bool isClientConnected(void);
	client_info_t getClient(void);
	
private:

	enum {
		WAIT_FOR_CLIENT,
		CONNECTING,
		CONNECTED,
		DISCONEECTD,
	};
	server_config_t& config;
	client_info_t client_info;
	int sock_fd;
	socklen_t length;
	Mutex clientMutex;
	void Run(void);
	static void *PthreadCallTx(void *tobject);
	static void *PthreadCallRx(void *tobject);
	void error(const char* msg);
	bool socketBind(void);
	bool enable_keepalive(int sockfd);
	void disablePipeBrokenSig(void);
};

#endif //__SERVERBASE_H__
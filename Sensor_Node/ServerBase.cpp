/*
 * File:   Server.cpp
 * Author: Mohammad Sadequr Rahman
 *
 *
 */
#include "ServerBase.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>


ServerBase::ServerBase(server_config_t& _config) :
	config(_config)
{
	disablePipeBrokenSig();
	if (!socketBind())
		error("socket bind error");
	if (listen(sock_fd, config.backlog) == -1)
		error("listen");
	//printf("Server init okay. waiting for connection\r\n");
}


ServerBase::~ServerBase()
{
}
/*
 * notifyDisconnect()
 * used to send notification to the socket state
 * machine that connection is broken from clinet. 
 * This function intented to use from the handle_rx and handle_tx
 */
void ServerBase::notifyDisconnect(void)
{
	clientMutex.Lock();
	client_info.is_a_client = false;
	clientMutex.Unlock();
}
/*
 * isClientConnected(void)
 * used to check is there any client connected
 */
bool ServerBase::isClientConnected(void)
{
	bool retValue = false;
	clientMutex.Lock();
	retValue = client_info.is_a_client;
	clientMutex.Unlock();

	return retValue;
}
/*
 * getClient()
 * used to get the client info structure 
 * which contain conection status and 
 * connected socket ID etc.
 */
client_info_t ServerBase::getClient(void)
{
	client_info_t this_client;
	clientMutex.Lock();
	this_client = client_info;
	clientMutex.Unlock();
	return this_client;
}
/*
 * Run()
 * this is a thread which actually run 
 * the socket state machine.
 */
void ServerBase::Run(void)
{
	socklen_t addrlen;
	addrlen = sizeof(struct sockaddr_storage);
	client_info.cState = WAIT_FOR_CLIENT;

	while (true)
	{
		switch (client_info.cState)
		{
		case WAIT_FOR_CLIENT:
			//printf("WAIT_FOR_CLIENT\r\n");
			if ((client_info.accepted_sock_id = accept(sock_fd, (struct sockaddr *) &client_info.client, &addrlen)) == -1) {
				if (errno == EINTR) {
					continue;
				}
				else {
					error("accept");
				}
				if (config.iskeepAlive)
					enable_keepalive(client_info.accepted_sock_id);
			}
			client_info.cState = CONNECTING;
			break;
		case CONNECTING:
			//printf("CONNECTING\r\n");
			struct sockaddr_in  *ptr;
			struct sockaddr_in6  *ptr1;
			char str[INET6_ADDRSTRLEN];
			if ((client_info.client).ss_family == AF_INET) {
				ptr = (struct sockaddr_in *) &(client_info.client);
				inet_ntop(AF_INET, &(ptr->sin_addr), str, sizeof(str));
			}
			else if ((client_info.client).ss_family == AF_INET6) {
				ptr1 = (struct sockaddr_in6 *) &(client_info.client);
				inet_ntop(AF_INET6, &(ptr1->sin6_addr), str, sizeof(str));
			}
			else
			{
				ptr = NULL;
				fprintf(stderr, "Address family is neither AF_INET nor AF_INET6\n");
			}
			/*if (ptr)
				printf("Connection from client %s\r\n", str);*/
			clientMutex.Lock();
			client_info.cState = CONNECTED;
			client_info.is_a_client = true;
			clientMutex.Unlock();

			if (int r = pthread_create(&client_info.txt_id, NULL, &PthreadCallTx, this) != 0) {
				fprintf(stderr, "Error = %d (%s)\n", r, strerror(r)); exit(1);
			}
			if (int r = pthread_create(&client_info.rxt_id, NULL, &PthreadCallRx, this) != 0) {
				fprintf(stderr, "Error = %d (%s)\n", r, strerror(r)); exit(1);
			}

			break;
		case CONNECTED:
			clientMutex.Lock();
			if (!client_info.is_a_client) client_info.cState = DISCONEECTD;
			clientMutex.Unlock();
			break;
		case DISCONEECTD:
			//printf("DISCONEECTD\r\n");
			clientMutex.Lock();
			pthread_cancel(client_info.rxt_id);
			pthread_cancel(client_info.txt_id);
			client_info.cState = WAIT_FOR_CLIENT;
			clientMutex.Unlock();
		default:
			break;
		}

	}
}

void * ServerBase::PthreadCallTx(void * tobject)
{
	reinterpret_cast<ServerBase*>(tobject)->handle_tx();
	return nullptr;
}

void * ServerBase::PthreadCallRx(void * tobject)
{
	reinterpret_cast<ServerBase*>(tobject)->handle_rx();
	return nullptr;
}

void ServerBase::error(const char * msg)
{
	perror(msg);
	exit(1);
}

bool ServerBase::socketBind(void)
{
	struct addrinfo hints;
	struct addrinfo *result;
	struct addrinfo *rptr;
	int s;
	memset(&hints, 0, sizeof(struct addrinfo));
	// configure the ip option

	switch (config.ipv)
	{
	case IPv4:
		hints.ai_family = AF_INET;
		break;
	case IPv6:
		hints.ai_family = AF_INET6;
		break;
	case UNSP:
	default:
		hints.ai_family = AF_UNSPEC;
		break;
	}
	// configure the socket type
	switch (config.socketType)
	{
	case TCP:
		hints.ai_socktype = SOCK_STREAM; /* Stream socket */
		break;
	case UDP:
		hints.ai_socktype = SOCK_DGRAM; /* Stream socket */
		break;
	default:
		break;
	}

	hints.ai_flags = AI_PASSIVE;    /* for wildcard IP address */

	if ((s = getaddrinfo(NULL, std::to_string(config.port).c_str(), &hints, &result)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		return false;
	}

	for (rptr = result; rptr != NULL; rptr = rptr->ai_next) {
		sock_fd = socket(rptr->ai_family, rptr->ai_socktype, rptr->ai_protocol);
		if (sock_fd == -1)
			continue;

		if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &config.isReusable, sizeof(int)) == -1)
			return false;


		if (bind(sock_fd, rptr->ai_addr, rptr->ai_addrlen) == 0)  // Success
			break;

		if (close(sock_fd) == -1)
			return false;
	}
	freeaddrinfo(result);

	return true;
}

bool ServerBase::enable_keepalive(int sockfd)
{
	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) != -1)
		return false;

	int idle = 1;
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(int)) != -1)
		return false;

	int interval = 1;
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(int)) != -1)
		return false;

	int maxpkt = 10;
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, &maxpkt, sizeof(int)) != -1)
		return false;

	return true;
}

void ServerBase::disablePipeBrokenSig(void)
{
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGPIPE);
	if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0)
		error("SIGPIPE");
}

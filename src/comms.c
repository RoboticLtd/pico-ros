#include "comms.h"

#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <sys/socket.h> /* for socket(), connect(), send(), and- recv() */
#include <ifaddrs.h>

#include <stdlib.h>

#include "utils.h"
#include "genmsg.h"
#include "slave_api.h"

#define MAX_CLIENTS_PENDING 5

bool comms_init()
{
	return TRUE;
}

bool comms_get_own_ip(char** ip_addr, const string interface)
{
	struct ifaddrs* ifAddrStruct = NULL;
	struct ifaddrs* ifa = NULL;
	void* tmpAddrPtr = NULL;
	*ip_addr = calloc(INET_ADDRSTRLEN, sizeof(char));

	getifaddrs(&ifAddrStruct);

	bool interface_found = FALSE;

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr->sa_family == AF_INET) //is IP4
		{
			tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			//PRINT("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
			if (strcmp(interface, ifa->ifa_name) == 0)
			{
				interface_found = TRUE;
				strcpy(*ip_addr, addressBuffer);
				break;
			}
		}
	}

	if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);

	if (!interface_found)
	{
		PRINT("Cannot find interface \"%s\" \n", interface);
	}

	return interface_found;
}
int comms_connect(const uri_t* server)
{
	int socketfd;
	struct sockaddr_in server_addr;

	/* Create a reliable, stream socket using TCP */
	socketfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketfd < 0) return -1;

	/* Construct the server address structure */
	memset(&server_addr, 0, sizeof(server_addr));           /* Zero out structure */
	server_addr.sin_family = AF_INET;                       /* Internet address family */
	server_addr.sin_addr.s_addr = inet_addr(server->ip);    /* Server IP address */
	server_addr.sin_port = htons(server->port);             /* Server port */

	/* Establish the connection to the echo server */
	if (connect(socketfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
		return -1;

	return socketfd;
}

int comms_send(int socketfd, char* data, int data_len)
{
	return send(socketfd, data, data_len, MSG_NOSIGNAL); /* NULL flags */
}

int comms_recv(int socketfd, char* data, int data_len)
{
	return recv(socketfd, data, data_len, MSG_NOSIGNAL); /* NULL flags */
}

void comms_disconnect(int socketfd)
{
	close(socketfd);
}


int comms_init_local_server(uri_t* server)
{
	int socketfd;
	socklen_t server_addr_len;
	struct sockaddr_in server_addr; /* Local server address */

	/* Create socket for incoming connections */
	socketfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketfd < 0) return -1;

	/* Construct local address structure */
	memset(&server_addr, 0, sizeof(server_addr));       /* Zero out structure */
	server_addr.sin_family = AF_INET;                   /* Internet address family */
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);    /* Any incoming interface */
	server_addr.sin_port = htons(0);                    /* Choose a random free port */

	server_addr_len = sizeof(server_addr);

	/* Bind to the local address */
	if (bind(socketfd,
	         (struct sockaddr*) &server_addr,
	         server_addr_len) < 0)
		return -1;

	/* Get assigned port */
	if (getsockname(socketfd,
	                (struct sockaddr*) &server_addr,
	                &server_addr_len) < 0)
		return -1;

	/* Set the port of the uri */
	server->port = (unsigned)ntohs(server_addr.sin_port);

	/* Mark the socket so it will listen for incoming connections */
	if (listen(socketfd, MAX_CLIENTS_PENDING) < 0)
		return -1;

	return socketfd;
}

int comms_accept_client(int socketfd)
{
	fd_set fds;
	struct timeval timeout;

	struct sockaddr_in client_addr;
	socklen_t client_len;
	int client_socket;

	/* Set time limit. */
	timeout.tv_sec = SELECT_TIMEOUT_SEC;
	timeout.tv_usec = SELECT_TIMEOUT_USEC;

	/* Create a descriptor set containing the sapi sockets.  */
	FD_ZERO(&fds);
	FD_SET(socketfd, &fds);

	if (select(socketfd + 1, &fds, NULL, NULL, &timeout) <= 0) return -1;

	if (!FD_ISSET(socketfd, &fds)) return -1;

	/* Set the size of the in-out parameter */
	client_len = sizeof(client_addr);

	/* Wait for a client to connect */
	client_socket = accept(socketfd,
	                       (struct sockaddr*) &client_addr,
	                       &client_len);

	if (client_socket < 0) return -1;

	return client_socket;
}



void comms_check_for_clients(const int* servers, int* clients, const int n)
{
	fd_set fds;
	struct timeval timeout;
	timeout.tv_sec = SELECT_TIMEOUT_SEC;
	timeout.tv_usec = SELECT_TIMEOUT_USEC;

	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	/* Create a descriptor set containing the sapi sockets.  */
	FD_ZERO(&fds);

	int i;
	for(i = 0; i < n; ++i)
	{
		FD_SET(servers[i], &fds);
		clients[i] = -1;
	}

	if (select(FD_SETSIZE, &fds, NULL, NULL, &timeout) <= 0) return;

	for(i = 0; i < n; ++i)
	{
		if (FD_ISSET(servers[i], &fds))
		{
			/* Wait for a client to connect */
			clients[i] = accept(servers[i],
		                        (struct sockaddr*) &client_addr,
		                        &client_len);
		}
	}
}

bool comms_check_for_data(list_t* conns)
{
	fd_set fds;
	struct timeval timeout;

	/* Set time limit. */
	timeout.tv_sec = SELECT_TIMEOUT_SEC;
	timeout.tv_usec = SELECT_TIMEOUT_USEC;

	/* Create a descriptor set containing our sockets. */
	FD_ZERO(&fds);

	list_item_t* it;
	for (it = conns->begin; it != NULL; it = it->next)
	{
		connection_t* con = it->data;
		FD_SET(con->socketfd, &fds);
	}

	if (select(FD_SETSIZE, &fds, NULL, NULL, &timeout) <= 0) return FALSE;

	for (it = conns->begin; it != NULL; it = it->next)
	{
		connection_t* con = it->data;
		con->pending = FD_ISSET(con->socketfd, &fds);
	}

	return TRUE;
}

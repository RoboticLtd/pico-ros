#include "tcpros.h"

#include <stdio.h>
#include <string.h>

#include "comms.h"
#include "utils.h"
#include "genmsg.h"


char tcp_package[TCP_MAX_PACKAGE_LEN];

void tcpros_send_subscriber_request(const int socketfd,
                                    const string callerid,
                                    const string topic,
                                    const message_type_t* msg,
                                    char* response,
                                    int* response_len)
{
	char* iter = tcp_package;

	tcpmsg_init(&iter);
	tcpmsg_add_field(&iter, "callerid", callerid);
	tcpmsg_add_field(&iter, "topic", topic);
	tcpmsg_add_field(&iter, "md5sum", msg->md5sum);
	tcpmsg_add_field(&iter, "type", msg->type);
	tcpmsg_finalize(tcp_package, iter - tcp_package);

	comms_send(socketfd, tcp_package, iter - tcp_package);
	*response_len = comms_recv(socketfd, response, NODE_MAX_RESPONSE_LEN);
}

void tcpros_parse_subscriber_request(const char* req,
                                     int req_len,
                                     char* callerid,
                                     char* topic,
                                     char* md5sum,
                                     char* type)
{
	const char *iter = req + 4;
	char *field = tcp_package;
	char *name = tcp_package + 512;
	char *value = tcp_package + 1024;

	while(iter - req < req_len)
   	{
   		int field_len = utils_get_int_from_bytes(iter, 4);
   		iter += 4;

   		if(field_len >= 512) return;

	   	strncpy(field, iter, field_len);
	   	field[field_len] = '\0';
	   	iter += field_len;

		SSCAN(field, "%[^'=']=%s", name, value);

		PRINT("[tcpros] name: %s value: %s\n", name, value);

		if(callerid != NULL &&
		   strcmp(name, "callerid") == 0)
		{
			strcpy(callerid, value);
		}
		else if(topic != NULL &&
		        strcmp(name, "topic") == 0)
		{
			strcpy(topic, value);
		}
		else if(md5sum != NULL &&
		        strcmp(name, "md5sum") == 0)
		{
			strcpy(md5sum, value);
		}
		else if(type != NULL &&
		        strcmp(name, "type") == 0)
		{
			strcpy(type, value);
		}
   	}
}

void tcpros_send_publisher_response(const int socketfd,
                                    const string callerid,
                                    const message_type_t* msg)
{
	char* iter = tcp_package;

	tcpmsg_init(&iter);
	tcpmsg_add_field(&iter, "callerid", callerid);
	tcpmsg_add_field(&iter, "md5sum", msg->md5sum);
	tcpmsg_add_field(&iter, "type", msg->type);
	tcpmsg_finalize(tcp_package, iter - tcp_package);

	comms_send(socketfd, tcp_package, iter - tcp_package);
}

void tcpros_parse_publisher_response(const char* resp,
                                     int resp_len,
                                     char* callerid,
                                     char* md5sum,
                                     char* type)
{
	const char *iter = resp + 4;
	char *field = tcp_package;
	char *name = tcp_package + 512;
	char *value = tcp_package + 1024;

	while(iter - resp < resp_len)
   	{
   		int field_len = utils_get_int_from_bytes(iter, 4);
   		iter += 4;

   		if(field_len >= 512) return;

	   	strncpy(field, iter, field_len);
	   	field[field_len] = '\0';
	   	iter += field_len;

		SSCAN(field, "%[^'=']=%s", name, value);

		PRINT("[tcpros] name: %s value: %s\n", name, value);

		if(callerid != NULL &&
		   strcmp(name, "callerid") == 0)
		{
			strcpy(callerid, value);
		}
		else if(md5sum != NULL &&
		        strcmp(name, "md5sum") == 0)
		{
			strcpy(md5sum, value);
		}
		else if(type != NULL &&
		        strcmp(name, "type") == 0)
		{
			strcpy(type, value);
		}
   	}
}

void tcpros_send_client_request(const int socketfd,
                                const string callerid,
                                const string service,
                                const service_type_t* srv,
                                const bool persistent,
                                char* response,
                                int* response_len)
{
	char* iter = tcp_package;

	tcpmsg_init(&iter);
	tcpmsg_add_field(&iter, "callerid", callerid);
	tcpmsg_add_field(&iter, "service", service);
	tcpmsg_add_field(&iter, "md5sum", srv->md5sum);
	tcpmsg_add_field(&iter, "type", srv->type);
	tcpmsg_add_field(&iter, "persistent", (persistent) ? "1" : "0");
	tcpmsg_finalize(tcp_package, iter - tcp_package);

	comms_send(socketfd, tcp_package, iter - tcp_package);
	*response_len = comms_recv(socketfd, response, NODE_MAX_RESPONSE_LEN);
}

void tcpros_parse_client_request(const char* req,
                                 int req_len,
                                 char* callerid,
                                 char* service,
                                 char* md5sum,
                                 char* type,
                                 bool* persistent,
                                 bool* probe)
{
	const char *iter = req + 4;
	char *field = tcp_package;
	char *name = tcp_package + 512;
	char *value = tcp_package + 1024;

	while(iter - req < req_len)
   	{
   		int field_len = utils_get_int_from_bytes(iter, 4);
   		iter += 4;

   		if(field_len >= 512) return;

	   	strncpy(field, iter, field_len);
	   	field[field_len] = '\0';
	   	iter += field_len;

		SSCAN(field, "%[^'=']=%s", name, value);

		PRINT("[tcpros] name: %s value: %s\n", name, value);

		if(callerid != NULL &&
		   strcmp(name, "callerid") == 0)
		{
			strcpy(callerid, value);
		}
		else if(service != NULL &&
		   strcmp(name, "service") == 0)
		{
			strcpy(service, value);
		}
		else if(md5sum != NULL &&
		        strcmp(name, "md5sum") == 0)
		{
			strcpy(md5sum, value);
		}
		else if(type != NULL &&
		        strcmp(name, "type") == 0)
		{
			strcpy(type, value);
		}
		else if(persistent != NULL &&
		        strcmp(name, "persistent") == 0)
		{
			*persistent = (strcmp(value, "1") == 0) ? TRUE : FALSE;
		}
		else if(probe != NULL &&
		        strcmp(name, "probe") == 0)
		{
			*probe = (strcmp(value, "1") == 0) ? TRUE : FALSE;
		}
   	}
}

void tcpros_send_server_response(const int socketfd,
                                 const string callerid,
                                 const service_type_t* srv)
{
	char* iter = tcp_package;

	tcpmsg_init(&iter);
	tcpmsg_add_field(&iter, "callerid", callerid);
	tcpmsg_add_field(&iter, "type", srv->type);
	tcpmsg_add_field(&iter, "md5sum", srv->md5sum);
	tcpmsg_finalize(tcp_package, iter - tcp_package);

	comms_send(socketfd, tcp_package, iter - tcp_package);
}

void tcpros_parse_server_response(const char* resp,
                                  int resp_len,
                                  char* callerid,
                                  char* md5sum,
                                  char* type)
{
	const char *iter = resp + 4;
	char *field = tcp_package;
	char *name = tcp_package + 512;
	char *value = tcp_package + 1024;

	while(iter - resp < resp_len)
   	{
   		int field_len = utils_get_int_from_bytes(iter, 4);
   		iter += 4;

   		if(field_len >= 512) return;

	   	strncpy(field, iter, field_len);
	   	field[field_len] = '\0';
	   	iter += field_len;

		SSCAN(field, "%[^'=']=%s", name, value);

		PRINT("[tcpros] name: %s value: %s\n", name, value);

		if(callerid != NULL &&
		   strcmp(name, "callerid") == 0)
		{
			strcpy(callerid, value);
		}
		else if(md5sum != NULL &&
		        strcmp(name, "md5sum") == 0)
		{
			strcpy(md5sum, value);
		}
		else if(type != NULL &&
		        strcmp(name, "type") == 0)
		{
			strcpy(type, value);
		}
   	}
}

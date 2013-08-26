#include "master_api.h"

#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "node.h"
#include "comms.h"
#include "utils.h"
#include "genmsg.h"

char mapi_body[MAPI_MAX_BODY_LEN];
char mapi_header[MAPI_MAX_HEADER_LEN];

bool mapi_call_registerPublisher(string caller_id,
		                         string topic,
		                         string topic_type,
		                         string caller_api,
		                         string response)
{
	xmlmsg_init(mapi_body);
	xmlmsg_open_tag(mapi_body, "methodCall");
		xmlmsg_add_tag_with_string(mapi_body, "methodName", "registerPublisher");
		xmlmsg_open_tag(mapi_body, "params");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_id);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", topic);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", topic_type);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_api);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

		xmlmsg_close_tag(mapi_body, "params");
	xmlmsg_close_tag(mapi_body, "methodCall");

	SPRINT(mapi_header, request_header_prototype, strlen(mapi_body));

	int socket = comms_connect(pico_node.master_uri);
	comms_send(socket, mapi_header, strlen(mapi_header));
	comms_send(socket, mapi_body, strlen(mapi_body));
	int n = comms_recv(socket, response, NODE_MAX_RESPONSE_LEN);
	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}

bool mapi_call_unregisterPublisher(string caller_id,
		                       	   string topic,
		                       	   string caller_api,
		                       	   string response)
{
	xmlmsg_init(mapi_body);
	xmlmsg_open_tag(mapi_body, "methodCall");
		xmlmsg_add_tag_with_string(mapi_body, "methodName", "unregisterPublisher");
		xmlmsg_open_tag(mapi_body, "params");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_id);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", topic);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_api);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

		xmlmsg_close_tag(mapi_body, "params");
	xmlmsg_close_tag(mapi_body, "methodCall");

	SPRINT(mapi_header, request_header_prototype, strlen(mapi_body));

	int socket = comms_connect(pico_node.master_uri);
	comms_send(socket, mapi_header, strlen(mapi_header));
	comms_send(socket, mapi_body, strlen(mapi_body));
	int n = comms_recv(socket, response, NODE_MAX_RESPONSE_LEN);
	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}

bool mapi_call_unregisterSubscriber(string caller_id,
		                       	    string topic,
		                       	    string caller_api,
		                       	    string response)
{
	xmlmsg_init(mapi_body);
	xmlmsg_open_tag(mapi_body, "methodCall");
		xmlmsg_add_tag_with_string(mapi_body, "methodName", "unregisterSubscriber");
		xmlmsg_open_tag(mapi_body, "params");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_id);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", topic);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_api);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

		xmlmsg_close_tag(mapi_body, "params");
	xmlmsg_close_tag(mapi_body, "methodCall");

	SPRINT(mapi_header, request_header_prototype, strlen(mapi_body));

	int socket = comms_connect(pico_node.master_uri);
	comms_send(socket, mapi_header, strlen(mapi_header));
	comms_send(socket, mapi_body, strlen(mapi_body));
	int n = comms_recv(socket, response, NODE_MAX_RESPONSE_LEN);
	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}

bool mapi_call_registerSubscriber(string caller_id,
			                      string topic,
			                      string topic_type,
			                      string caller_api,
			                      string response)
{
	xmlmsg_init(mapi_body);
	xmlmsg_open_tag(mapi_body, "methodCall");
		xmlmsg_add_tag_with_string(mapi_body, "methodName", "registerSubscriber");
		xmlmsg_open_tag(mapi_body, "params");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_id);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", topic);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", topic_type);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_api);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

		xmlmsg_close_tag(mapi_body, "params");
	xmlmsg_close_tag(mapi_body, "methodCall");

	SPRINT(mapi_header, request_header_prototype, strlen(mapi_body));

	int socket = comms_connect(pico_node.master_uri);
	comms_send(socket, mapi_header, strlen(mapi_header));
	comms_send(socket, mapi_body, strlen(mapi_body));
	int n = comms_recv(socket, response, NODE_MAX_RESPONSE_LEN);
	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}

list_t* mapi_parse_registerSubscriber_response(const string response)
{
	char* iter = response;

	list_t *uris = list_create();
	uri_t* uri;

	while((iter = strstr(iter, "http://")) != NULL)
	{
		uri = uri_create_from_http_string(iter);

		PRINT("[mapi] Slave APIs at %s:%hu\n", uri->ip, uri->port);

		list_insert_item(uris, uri);

		iter += MIN_URI_PARSE_ITER_STEP;
	}

	return uris;
}

bool mapi_call_registerService(string caller_id,
                               string service,
                               string service_api,
                               string caller_api,
                               string response)
{
	xmlmsg_init(mapi_body);
	xmlmsg_open_tag(mapi_body, "methodCall");
		xmlmsg_add_tag_with_string(mapi_body, "methodName", "registerService");
		xmlmsg_open_tag(mapi_body, "params");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_id);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", service);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", service_api);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_api);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

		xmlmsg_close_tag(mapi_body, "params");
	xmlmsg_close_tag(mapi_body, "methodCall");

	SPRINT(mapi_header, request_header_prototype, strlen(mapi_body));

	int socket = comms_connect(pico_node.master_uri);
	comms_send(socket, mapi_header, strlen(mapi_header));
	comms_send(socket, mapi_body, strlen(mapi_body));
	int n = comms_recv(socket, response, NODE_MAX_RESPONSE_LEN);
	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}


bool mapi_call_unregisterService(string caller_id,
		                       	 string service,
		                       	 string caller_api,
		                       	 string response)
{
	xmlmsg_init(mapi_body);
	xmlmsg_open_tag(mapi_body, "methodCall");
		xmlmsg_add_tag_with_string(mapi_body, "methodName", "registerSubscriber");
		xmlmsg_open_tag(mapi_body, "params");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_id);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", service);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_api);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

		xmlmsg_close_tag(mapi_body, "params");
	xmlmsg_close_tag(mapi_body, "methodCall");

	SPRINT(mapi_header, request_header_prototype, strlen(mapi_body));

	int socket = comms_connect(pico_node.master_uri);
	comms_send(socket, mapi_header, strlen(mapi_header));
	comms_send(socket, mapi_body, strlen(mapi_body));
	int n = comms_recv(socket, response, NODE_MAX_RESPONSE_LEN);
	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}

bool mapi_call_lookupService(string caller_id,
                             string service,
                             string response)
{
	xmlmsg_init(mapi_body);
	xmlmsg_open_tag(mapi_body, "methodCall");
		xmlmsg_add_tag_with_string(mapi_body, "methodName", "lookupService");
		xmlmsg_open_tag(mapi_body, "params");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_id);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", service);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

		xmlmsg_close_tag(mapi_body, "params");
	xmlmsg_close_tag(mapi_body, "methodCall");

	SPRINT(mapi_header, request_header_prototype, strlen(mapi_body));

	int socket = comms_connect(pico_node.master_uri);
	comms_send(socket, mapi_header, strlen(mapi_header));
	comms_send(socket, mapi_body, strlen(mapi_body));
	int n = comms_recv(socket, response, NODE_MAX_RESPONSE_LEN);
	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}

uri_t* mapi_parse_lookupService_response(const string response)
{
	char* iter = response;

	uri_t* uri;

	iter = strstr(iter, "<string>rosrpc://");
	if(iter == NULL) return NULL;

	uri = uri_create_from_rosrpc_string(iter);
	return uri;
}

// Param server
bool mapi_call_setParam(const string caller_id,
     	                const string param_name,
        	            const string param_tag,
            	        const string param_value,
            	        string response)
{
	xmlmsg_init(mapi_body);
	xmlmsg_open_tag(mapi_body, "methodCall");
		xmlmsg_add_tag_with_string(mapi_body, "methodName", "setParam");
		xmlmsg_open_tag(mapi_body, "params");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_id);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", param_name);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, param_tag, param_value);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

		xmlmsg_close_tag(mapi_body, "params");
	xmlmsg_close_tag(mapi_body, "methodCall");

	SPRINT(mapi_header, request_header_prototype, strlen(mapi_body));

	int socket = comms_connect(pico_node.master_uri);
	comms_send(socket, mapi_header, strlen(mapi_header));
	comms_send(socket, mapi_body, strlen(mapi_body));
	int n = comms_recv(socket, response, NODE_MAX_RESPONSE_LEN);
	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}

bool mapi_call_getParam(const string caller_id,
     	                const string param_name,
            	        string response)
{
	xmlmsg_init(mapi_body);
	xmlmsg_open_tag(mapi_body, "methodCall");
		xmlmsg_add_tag_with_string(mapi_body, "methodName", "getParam");
		xmlmsg_open_tag(mapi_body, "params");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_id);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", param_name);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

		xmlmsg_close_tag(mapi_body, "params");
	xmlmsg_close_tag(mapi_body, "methodCall");

	SPRINT(mapi_header, request_header_prototype, strlen(mapi_body));

	int socket = comms_connect(pico_node.master_uri);
	comms_send(socket, mapi_header, strlen(mapi_header));
	comms_send(socket, mapi_body, strlen(mapi_body));
	int n = comms_recv(socket, response, NODE_MAX_RESPONSE_LEN);
	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}

string mapi_parse_getParam_response(const string response)
{
	char* iter = response;

	iter = strstr(iter, "<value><int>1");
	if(iter == NULL) return NULL;

	iter += 25; //strlen(<value><int></int></value>)

	iter = strstr(iter, "<value>");
	iter += 15; //strlen(<value></value>)

	iter = strstr(iter, "<value>") + 7; //strlen(<value>)
	*strstr(iter, "</value>") = '\0';

	return iter;
}

bool mapi_call_hasParam(const string caller_id,
     	                const string param_name,
            	        string response)
{
	xmlmsg_init(mapi_body);
	xmlmsg_open_tag(mapi_body, "methodCall");
		xmlmsg_add_tag_with_string(mapi_body, "methodName", "hasParam");
		xmlmsg_open_tag(mapi_body, "params");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_id);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", param_name);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

		xmlmsg_close_tag(mapi_body, "params");
	xmlmsg_close_tag(mapi_body, "methodCall");

	SPRINT(mapi_header, request_header_prototype, strlen(mapi_body));

	int socket = comms_connect(pico_node.master_uri);
	comms_send(socket, mapi_header, strlen(mapi_header));
	comms_send(socket, mapi_body, strlen(mapi_body));
	int n = comms_recv(socket, response, NODE_MAX_RESPONSE_LEN);
	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}

bool mapi_call_deleteParam(const string caller_id,
     	                   const string param_name,
            	           string response)
{
	xmlmsg_init(mapi_body);

	xmlmsg_open_tag(mapi_body, "methodCall");
		xmlmsg_add_tag_with_string(mapi_body, "methodName", "deleteParam");
		xmlmsg_open_tag(mapi_body, "params");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", caller_id);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

			xmlmsg_open_tag(mapi_body, "param");
				xmlmsg_open_tag(mapi_body, "value");
					xmlmsg_add_tag_with_string(mapi_body, "string", param_name);
				xmlmsg_close_tag(mapi_body, "value");
			xmlmsg_close_tag(mapi_body, "param");

		xmlmsg_close_tag(mapi_body, "params");
	xmlmsg_close_tag(mapi_body, "methodCall");

	SPRINT(mapi_header, request_header_prototype, strlen(mapi_body));

	int socket = comms_connect(pico_node.master_uri);
	comms_send(socket, mapi_header, strlen(mapi_header));
	comms_send(socket, mapi_body, strlen(mapi_body));
	int n = comms_recv(socket, response, NODE_MAX_RESPONSE_LEN);
	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}

bool mapi_parse_deleteParam_response(const string response)
{
	char* iter = response;

	iter = strstr(iter, "<value><int>1");
	if(iter == NULL) return FALSE;
	else return TRUE;
}

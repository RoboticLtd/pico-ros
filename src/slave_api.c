#include "slave_api.h"

#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "node.h"
#include "utils.h"
#include "comms.h"
#include "genmsg.h"

char sapi_body[SAPI_MAX_BODY_LEN];
char sapi_header[SAPI_MAX_HEADER_LEN];

void sapi_respond_getBusInfo(int client_socket, list_t* sub, list_t* pub)
{
	xmlmsg_init(sapi_body);

	xmlmsg_open_method_response(sapi_body);
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_add_tag_with_ushort(sapi_body, "i4", 1);
		xmlmsg_close_tag(sapi_body, "value");

		xmlmsg_open_tag(sapi_body, "value");
		xmlmsg_close_tag(sapi_body, "value");

		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_open_tag(sapi_body, "array");
				xmlmsg_open_tag(sapi_body, "data");

				list_item_t* it;
				for(it = sub->begin; it != NULL; it = it->next)
					xmlmsg_add_topic(sapi_body, (topic_t*)it->data);

				for(it = pub->begin; it != NULL; it = it->next)
					xmlmsg_add_topic(sapi_body, (topic_t*)it->data);

				xmlmsg_close_tag(sapi_body, "data");
			xmlmsg_close_tag(sapi_body, "array");
		xmlmsg_close_tag(sapi_body, "value");
	xmlmsg_close_method_response(sapi_body);

	SPRINT(sapi_header, response_header_prototype, strlen(sapi_body));

	comms_send(client_socket, sapi_header, strlen(sapi_header));
	comms_send(client_socket, sapi_body, strlen(sapi_body));
}

void sapi_respond_getPid(int client_socket, unsigned int pid)
{
	xmlmsg_init(sapi_body);

	xmlmsg_open_method_response(sapi_body);
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_add_tag_with_ushort(sapi_body, "i4", 1);
		xmlmsg_close_tag(sapi_body, "value");
		xmlmsg_open_tag(sapi_body, "value");
		xmlmsg_close_tag(sapi_body, "value");
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_add_tag_with_uint(sapi_body, "i4", pid);
		xmlmsg_close_tag(sapi_body, "value");
	xmlmsg_close_method_response(sapi_body);

	SPRINT(sapi_header, response_header_prototype, strlen(sapi_body));

	comms_send(client_socket, sapi_header, strlen(sapi_header));
	comms_send(client_socket, sapi_body, strlen(sapi_body));
}

void sapi_respond_getPublications_getSubscriptions(int client_socket, list_t* pub)
{
	xmlmsg_init(sapi_body);

	xmlmsg_open_method_response(sapi_body);
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_add_tag_with_ushort(sapi_body, "i4", 1);
		xmlmsg_close_tag(sapi_body, "value");

		xmlmsg_open_tag(sapi_body, "value");
		xmlmsg_close_tag(sapi_body, "value");

		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_open_tag(sapi_body, "array");
				xmlmsg_open_tag(sapi_body, "data");

				list_item_t* it;

				for(it = pub->begin; it != NULL; it = it->next)
				{
					topic_t* t = it->data;
					xmlmsg_open_tag(sapi_body, "value");
						xmlmsg_open_tag(sapi_body, "array");
							xmlmsg_open_tag(sapi_body, "data");

								xmlmsg_add_tag_with_string(sapi_body, "value", t->name);
								xmlmsg_add_tag_with_string(sapi_body, "value", (string)(t->msg_type->type));

							xmlmsg_close_tag(sapi_body, "data");
						xmlmsg_close_tag(sapi_body, "array");
					xmlmsg_close_tag(sapi_body, "value");
				}

				xmlmsg_close_tag(sapi_body, "data");
			xmlmsg_close_tag(sapi_body, "array");
		xmlmsg_close_tag(sapi_body, "value");
	xmlmsg_close_method_response(sapi_body);

	SPRINT(sapi_header, response_header_prototype, strlen(sapi_body));

	comms_send(client_socket, sapi_header, strlen(sapi_header));
	comms_send(client_socket, sapi_body, strlen(sapi_body));
}


void sapi_respond_publisherUpdate(int client_socket)
{
	xmlmsg_init(sapi_body);

	xmlmsg_open_method_response(sapi_body);
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_add_tag_with_ushort(sapi_body, "i4", 1);
		xmlmsg_close_tag(sapi_body, "value");
		xmlmsg_open_tag(sapi_body, "value");
		xmlmsg_close_tag(sapi_body, "value");
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_add_tag_with_ushort(sapi_body, "i4", 0);
		xmlmsg_close_tag(sapi_body, "value");
	xmlmsg_close_method_response(sapi_body);

	SPRINT(sapi_header, response_header_prototype, strlen(sapi_body));

	comms_send(client_socket, sapi_header, strlen(sapi_header));
	comms_send(client_socket, sapi_body, strlen(sapi_body));
}


list_t* sapi_parse_publisherUpdate_request(const string request, string topic)
{
	char *iter = strstr(request, "<string>/master");
	if (iter == NULL) return NULL;

	iter = strstr(iter, "<string>");
    iter += 17; // strlen(<string></string>)

    iter = strstr(iter, "<string>");
	SSCAN(iter, "<string>%[^<]", topic);
    iter += 17; // strlen(<string></string>)

    list_t* l = list_create();
    while((iter = strstr(iter, "http://")) != NULL)
    {
    	list_insert_item(l, uri_create_from_http_string(iter));

		iter += MIN_URI_PARSE_ITER_STEP;
    }

    return l;
}

bool sapi_call_requestTopic(const uri_t* slave_uri,
                            const string caller_id,
                       		const string topic,
                       		const string protocols,
                       		string response)
{
	xmlmsg_init(sapi_body);
	xmlmsg_open_tag(sapi_body, "methodCall");
		xmlmsg_add_tag_with_string(sapi_body, "methodName", "requestTopic");
		xmlmsg_open_tag(sapi_body, "params");

			xmlmsg_open_tag(sapi_body, "param");
				xmlmsg_add_tag_with_string(sapi_body, "value", caller_id);
			xmlmsg_close_tag(sapi_body, "param");

			xmlmsg_open_tag(sapi_body, "param");
				xmlmsg_add_tag_with_string(sapi_body, "value", topic);
			xmlmsg_close_tag(sapi_body, "param");

			xmlmsg_open_tag(sapi_body, "param");
				xmlmsg_open_tag(sapi_body, "value");
					xmlmsg_open_tag(sapi_body, "array");
						xmlmsg_open_tag(sapi_body, "data");
							xmlmsg_open_tag(sapi_body, "value");
								xmlmsg_open_tag(sapi_body, "array");
									xmlmsg_open_tag(sapi_body, "data");
										xmlmsg_add_tag_with_string(sapi_body, "value", protocols);
									xmlmsg_close_tag(sapi_body, "data");
								xmlmsg_close_tag(sapi_body, "array");
							xmlmsg_close_tag(sapi_body, "value");
						xmlmsg_close_tag(sapi_body, "data");
					xmlmsg_close_tag(sapi_body, "array");
				xmlmsg_close_tag(sapi_body, "value");
			xmlmsg_close_tag(sapi_body, "param");

		xmlmsg_close_tag(sapi_body, "params");
	xmlmsg_close_tag(sapi_body, "methodCall");

	SPRINT(sapi_header, request_header_prototype, strlen(sapi_body));

	int socket = comms_connect(slave_uri);
	if(socket < 0) return FALSE;

	comms_send(socket, sapi_header, strlen(sapi_header));
	comms_send(socket, sapi_body, strlen(sapi_body));

	// Make sure the whole package is received
	int n = 0;
	do
	{
		n += comms_recv(socket, response + n, NODE_MAX_RESPONSE_LEN);
	} while(n < COMMS_MIN_PACKAGE_LEN);

	comms_disconnect(socket);

	response[n] = '\0';

	return n > 0;
}

void sapi_respond_requestTopic(int client_socket,
                               const uri_t *tcpros_server_uri)
{
	xmlmsg_init(sapi_body);
	xmlmsg_open_method_response(sapi_body);
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_add_tag_with_string(sapi_body, "i4", "1"); /* Status OK */
		xmlmsg_close_tag(sapi_body, "value");
		xmlmsg_open_tag(sapi_body, "value");
		xmlmsg_close_tag(sapi_body, "value");
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_open_tag(sapi_body, "array");
				xmlmsg_open_tag(sapi_body, "data");
					xmlmsg_add_tag_with_string(sapi_body, "value", "TCPROS");
					xmlmsg_add_tag_with_string(sapi_body, "value", tcpros_server_uri->ip);
					xmlmsg_open_tag(sapi_body, "value");
					xmlmsg_add_tag_with_ushort(sapi_body, "i4", tcpros_server_uri->port);
					xmlmsg_close_tag(sapi_body, "value");
				xmlmsg_close_tag(sapi_body, "data");
			xmlmsg_close_tag(sapi_body, "array");
		xmlmsg_close_tag(sapi_body, "value");
	xmlmsg_close_method_response(sapi_body);

	SPRINT(sapi_header, response_header_prototype, strlen(sapi_body));

	comms_send(client_socket, sapi_header, strlen(sapi_header));
	comms_send(client_socket, sapi_body, strlen(sapi_body));
}


void sapi_parse_requestTopic_request(const string request,
                                     bool *supported_protocol)
{
	*supported_protocol = (strstr(request, "TCPROS") != NULL);
}

void sapi_parse_requestTopic_response(const string response,
                                      uri_t **tcpros_uri)
{
	char hostname[SAPI_MAX_URI_LEN];
	unsigned short port;
	char *iter = response;

	/* Check if TCPROS is supported */
	iter = strstr(iter, "<value>TCPROS");
	if(iter == NULL)
	{
		PRINT("Unsupported protocol. Last received response:\n%s\n", response);
		return;
	}

	/* Create uri */
	/* Read the host name */
	++iter;
	iter = strstr(iter, "<value>");
	SSCAN(iter, "<value>%[^<]", hostname);

	/* Get the port number */
	++iter;
	iter = strstr(iter, "<i4>");
	SSCAN(iter, "<i4>%hu", &port);

	*tcpros_uri = uri_create(hostname, port);
}

void sapi_respond_getMasterUri(int client_socket, uri_t* uri)
{
	xmlmsg_init(sapi_body);

	xmlmsg_open_method_response(sapi_body);
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_add_tag_with_ushort(sapi_body, "i4", 1);
		xmlmsg_close_tag(sapi_body, "value");
		xmlmsg_open_tag(sapi_body, "value");
		xmlmsg_close_tag(sapi_body, "value");
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_add_tag_with_string(sapi_body, "string", uri_get_http_string(uri));
		xmlmsg_close_tag(sapi_body, "value");
	xmlmsg_close_method_response(sapi_body);

	SPRINT(sapi_header, response_header_prototype, strlen(sapi_body));

	comms_send(client_socket, sapi_header, strlen(sapi_header));
	comms_send(client_socket, sapi_body, strlen(sapi_body));
}

void sapi_respond_shutdown(int client_socket)
{
	xmlmsg_init(sapi_body);

	xmlmsg_open_method_response(sapi_body);
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_add_tag_with_ushort(sapi_body, "i4", 1);
		xmlmsg_close_tag(sapi_body, "value");
		xmlmsg_open_tag(sapi_body, "value");
		xmlmsg_close_tag(sapi_body, "value");
		xmlmsg_open_tag(sapi_body, "value");
			xmlmsg_add_tag_with_ushort(sapi_body, "i4", 0);
		xmlmsg_close_tag(sapi_body, "value");
	xmlmsg_close_method_response(sapi_body);

	SPRINT(sapi_header, response_header_prototype, strlen(sapi_body));

	comms_send(client_socket, sapi_header, strlen(sapi_header));
	comms_send(client_socket, sapi_body, strlen(sapi_body));
}

void sapi_parse_shutdown_request(const string request, string msg)
{
	char *iter = strstr(request, "<string>");
	if (iter == NULL) return;

	iter = strstr(iter, "<string>");
    iter += 17; // strlen(<string></string>)

    iter = strstr(iter, "<string>");
	SSCAN(iter, "<string>%[^<]", msg);
}



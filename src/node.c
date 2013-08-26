#include "node.h"

#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include "comms.h"
#include "utils.h"
#include "tcpros.h"
#include "genmsg.h"
#include "slave_api.h"
#include "master_api.h"


node_t pico_node;
char request[NODE_MAX_REQUEST_LEN];
char response[NODE_MAX_RESPONSE_LEN];

//const string INTERFACE = "wlan0";
const string INTERFACE = "eth0";

void node_free(node_t* n)
{
	list_free(n->published_topics, (LIST_FREE_FUNC)topic_free);
	list_free(n->subscribed_topics, (LIST_FREE_FUNC)topic_free);
	list_free(n->service_servers, (LIST_FREE_FUNC)service_free);

	uri_free(n->sapi_server_uri);
	uri_free(n->tcpros_server_uri);
	uri_free(n->rosrpc_server_uri);

	free(n->ip);
	free(n->name);
}

bool node_init(node_t* n, string node_name)
{
	if(!comms_get_own_ip(&n->ip, INTERFACE)) return FALSE;

	n->name = (string)calloc(sizeof(char), strlen(node_name) + 1);
	strcpy(n->name, node_name);

	n->exit = FALSE;

	n->master_uri = uri_create("127.0.0.1", 11311);

	// Check if the master is alive
	int master_socket = comms_connect(n->master_uri);
	if(master_socket < 0)
	{
		PRINT("[node] Unable to connect to master.\n");
		return FALSE;
	}
	else
	{
		comms_disconnect(master_socket);
	}

	n->sapi_server_uri = uri_create(n->ip, 0);
	n->server_sockets[SAPI_SERVER] =
	    comms_init_local_server(n->sapi_server_uri);
	if (n->server_sockets[SAPI_SERVER] < 0) return FALSE;

	n->tcpros_server_uri = uri_create(n->ip, 0);
	n->server_sockets[TCPROS_SERVER] =
	    comms_init_local_server(n->tcpros_server_uri);
	if (n->server_sockets[TCPROS_SERVER] < 0) return FALSE;

	n->rosrpc_server_uri = uri_create(n->ip, 0);
	n->server_sockets[ROSRPC_SERVER] =
	    comms_init_local_server(n->rosrpc_server_uri);
	if (n->server_sockets[ROSRPC_SERVER] < 0) return FALSE;

	// Topics
	n->published_topics = list_create();
	n->subscribed_topics = list_create();

	// Services
	n->service_servers = list_create();

	// Print info
	string str_uri = uri_get_http_string(n->sapi_server_uri);
	PRINT("[node] SAPI server uri: %s\n", str_uri);
	free(str_uri);

	str_uri = uri_get_http_string(n->tcpros_server_uri);
	PRINT("[node] TCPROS server uri: %s\n", str_uri);
	free(str_uri);

	str_uri = uri_get_rosrpc_string(n->rosrpc_server_uri);
	PRINT("[node] ROSRPC server uri: %s\n", str_uri);
	free(str_uri);

	return TRUE;
}

void node_stop(node_t* n)
{
	string api = uri_get_http_string(n->sapi_server_uri);
	list_item_t* it;
	for(it = n->published_topics->begin; it != NULL; it = it->next)
	{
		topic_t* t = it->data;
		mapi_call_unregisterPublisher(n->name, t->name, api, response);
	}

	for(it = n->subscribed_topics->begin; it != NULL; it = it->next)
	{
		topic_t* t = it->data;
		mapi_call_unregisterSubscriber(n->name, t->name, api, response);
	}
	free(api);

	api = uri_get_rosrpc_string(n->rosrpc_server_uri);
	for(it = n->service_servers->begin; it != NULL; it = it->next)
	{
		service_server_t* s = it->data;
		mapi_call_unregisterService(n->name, s->name, api, response);
	}
	free(api);

	node_free(n);
}

void node_spin(node_t* n)
{
	comms_check_for_clients(n->server_sockets,
	                        n->client_sockets,
	                        NODE_SERVERS_COUNT);

	node_spin_sapi_server(n);
	node_spin_tcpros_server(n);
	node_spin_rpcros_server(n);

	list_item_t* it;
	for(it = n->subscribed_topics->begin; it != NULL; it = it->next)
	{
		node_spin_subscribed_topic(it->data);
	}

	for(it = n->service_servers->begin; it != NULL; it = it->next)
	{
		node_spin_service_server(it->data);
	}
}

void node_spin_sapi_server(node_t* n)
{
	int client_socket = n->client_sockets[SAPI_SERVER];
	if(client_socket < 0) return;

	int len = comms_recv(client_socket, request, NODE_MAX_REQUEST_LEN);
	if(len < 0) return;

	char* iter = strstr(request, "<methodName>");
    if(iter == NULL) return;

    /* Read the method name */
    char method_name[NODE_MAX_METHOD_NAME];
    SSCAN(iter, "<methodName>%[^<]", method_name);

    PRINT("[node] SAPI server received request: %s\n", method_name);

    /*  Figure out which sapi function to call;
    	6 is the most unique column.
	*/
    switch(method_name[6])
    {
		case 'I':
			/* getBusInfo */
			node_getBusInfo(n, request, client_socket);

			break;
		case 'S':
			/* getBusStats */
			node_getBusStats(n, request, client_socket);

			break;
		case '\0':
			/* getPid */
			node_getPid(n, request, client_socket);

			break;
		case 'l':
			/* getPublications */
			node_getPublications(n, request, client_socket);

			break;
		case 's':
			/* getSubscriptions */
			node_getSubscriptions(n, request, client_socket);

			break;
		case 'p':
			/* paramUpdate */
			node_paramUpdate(n, request, client_socket);

			break;
		case 'h':
			/* publisherUpdate */
			node_publisherUpdate(n, request, client_socket);
			break;
		case 't':

			if(method_name[0] == 'r')
			{
				/* requestTopic */
				node_requestTopic(n, request, client_socket);

			}
			else
			{
				/* getMasteruri_t */
				node_getMasterUri(n, request, client_socket);
			}

			break;
		case 'w':
			/* shutdown */
			node_shutdown(n, request, client_socket);
			break;
		default:
			comms_disconnect(client_socket);
			return;

    }

	comms_disconnect(client_socket);
}

void node_spin_tcpros_server(node_t* n)
{
	int client_socket = n->client_sockets[TCPROS_SERVER];
    if(client_socket < 0) return;

    int len = comms_recv(client_socket, request, NODE_MAX_REQUEST_LEN);
	if(len < 0) return;

	char callerid[128];
	char topic[128];
	char md5sum[128];
	char type[128];

	tcpros_parse_subscriber_request(request, len, callerid, topic, md5sum, type);

   	topic_t* t = node_get_published_topic_by_name(n, topic);

   	/* The topic is not published */
   	if(t == NULL)
	{
		PRINT("[node] Topic is not published.\n");
		return;
	}

   	/* The message is not supported */
   	if(strcmp(t->msg_type->md5sum, md5sum) != 0 &&
   	   strcmp(md5sum, "*") != 0) //rostopic hz
   	{
   		PRINT("[node] MD5SUM mismatch.\n");
   		return;
   	}

	connection_t* c = topic_get_connection_by_name(t, callerid);

	if(c == NULL)
	{
		c = connection_create(NULL, callerid, client_socket);
		topic_add_connection(t, c);
	}

   	tcpros_send_publisher_response(client_socket, n->name, t->msg_type);
}

void node_spin_rpcros_server(node_t* n)
{
	int client_socket = n->client_sockets[ROSRPC_SERVER];
    if(client_socket < 0) return;

    int len = comms_recv(client_socket, request, NODE_MAX_REQUEST_LEN);
	if(len < 0) return;

	char callerid[128];
	char service[128];
	char md5sum[128];
	char type[128];
	bool persistent = FALSE;
	bool probe = FALSE;

	tcpros_parse_client_request(request, len,
	                            callerid,
	                            service,
	                            md5sum,
	                            type,
	                            &persistent,
	                            &probe);

	service_server_t* s = node_get_service_server_by_name(n, service);

	/* The service is not advertised */
	if(s == NULL)
	{
		PRINT("[node] Service is not advertised.\n");
		return;
	}

	/* The service type is not supported */
   	if(strcmp(s->srv_type->md5sum, md5sum) != 0 && !probe)
   	{
   		PRINT("[node] MD5SUM mismatch.\n");
   		return;
   	}

   	tcpros_send_server_response(client_socket, n->name, s->srv_type);

   	// If this is probe request the connection
   	// should be closed after the response.
   	if(probe)
   	{
   		comms_disconnect(client_socket);
   		return;
   	}

   	connection_t* con = connection_create(NULL, callerid, client_socket);

   	if(!persistent)
	{
		len = comms_recv(client_socket, request, NODE_MAX_RESPONSE_LEN);
	   	if(len <= 0)
	   	{
	   		comms_disconnect(client_socket);
	   		connection_free(con);
	   		return;
	   	}

	   	void* srv = s->srv_type->deserialize_req(request + 4);
   		s->callback(srv);
		len = s->srv_type->serialize_resp(response, srv);

	   	comms_send(con->socketfd, response, len);
		comms_disconnect(con->socketfd);
		connection_free(con);
	}
	else
	{
		service_add_connection(s, con);
	}
}

void node_spin_subscribed_topic(topic_t* t)
{
	comms_check_for_data(t->conns);

	int len;
	list_item_t* it;
	for(it = t->conns->begin; it != NULL; it = it->next)
	{
		connection_t* con = it->data;
		if (!con->pending) continue;

		len = comms_recv(con->socketfd, response, NODE_MAX_RESPONSE_LEN);
		if(len <= 0) continue;

		// Read the last received message
		// NOTE: Could add message queue size
		int msg_len = 0, offset = 0;
		do
		{
			offset += msg_len;
			msg_len = utils_get_int_from_bytes(response + offset, sizeof(int32_t)) + sizeof(int32_t);
		}
		while(offset + msg_len < len);

		void* msg = t->msg_type->deserialize(response + offset);
		t->callback(msg);
		t->msg_type->free_msg(msg);
	}
}

void node_publish_topic(topic_t* t, const void* msg)
{
	int n = t->msg_type->serialize(request, msg);
	list_item_t* iter = t->conns->begin;
	connection_t* con;

	while(iter != NULL)
	{
		con = iter->data;
		// Do not move at the end, because the connection might be removed.
		iter = iter->next;

		int len = comms_send(con->socketfd, request, n);
		if(len <= 0)
		{
			PRINT("[node] Client %s has closed connection at topic %s.\n", con->name, t->name);
			topic_remove_connection(t, con);
		}
	}
}

// NOTE: Possibly implement latching
topic_t* node_advertise(node_t* n, string name, string type)
{
	// Check if the node is already advertised
	topic_t* t = node_get_published_topic_by_name(n, name);
	if(t != NULL) return t;

	t = topic_create(name, OUTBOUND, type, NULL);
	if(t == NULL) return NULL;

	list_insert_item(n->published_topics, t);

	string sapi_uri_str = uri_get_http_string(n->sapi_server_uri);
	mapi_call_registerPublisher(n->name, name, type, sapi_uri_str, response);
	free(sapi_uri_str);

	return t;
}

topic_t* node_subscribe(node_t* n, string name, string type, void (*callback)(const void*))
{
	topic_t* t = topic_create(name, INBOUND, type, callback);
	list_insert_item(n->subscribed_topics, t);

	/* Register subscriber with the master */
	string sapi_uri_str = uri_get_http_string(n->sapi_server_uri);
	mapi_call_registerSubscriber(n->name, name, type, sapi_uri_str, response);
	free(sapi_uri_str);

	list_t *sapi_uris = mapi_parse_registerSubscriber_response(response);

	node_connect_to_publishers(n, t, sapi_uris);

	list_free(sapi_uris, (LIST_FREE_FUNC)uri_free);

	return t;
}

void node_connect_to_publishers(node_t *n, topic_t *t, list_t *pubs)
{
	uri_t* tcpros_uri = NULL;
	char callerid[NODE_MAX_NAME_LEN];
	int response_len;

	list_item_t* iter;
	for(iter = pubs->begin; iter != NULL; iter = iter->next)
	{
		/* Call sapi requestTopic for each publisher */
		if(!sapi_call_requestTopic(iter->data, n->name, t->name, "TCPROS", response))
			continue;

		/* Get the uri of the publisher's TCPROS server */
		sapi_parse_requestTopic_response(response, &tcpros_uri);

		/* Initialise the TCPROS connection */
		int socketfd = comms_connect(tcpros_uri);

		/* Establish TCPROS connection */
		tcpros_send_subscriber_request(socketfd, n->name,
		                 t->name, t->msg_type,
		                 response, &response_len);

		tcpros_parse_publisher_response(response, response_len, callerid, NULL, NULL);

		/* Add connection to the topic */
		connection_t *con = connection_create(iter->data, callerid, socketfd);
		topic_add_connection(t, con);
	}
}

service_server_t* node_service_server(node_t* n, string name, string type, void (*callback)(void*))
{
	service_server_t* s = service_create(name, type, FALSE, callback);
	if(s == NULL) return NULL;

	list_insert_item(n->service_servers, s);

	string sapi_uri_str = uri_get_http_string(n->sapi_server_uri);
	string rosrpc_uri_str = uri_get_rosrpc_string(n->rosrpc_server_uri);

	mapi_call_registerService(n->name, name, rosrpc_uri_str, sapi_uri_str, response);

	free(sapi_uri_str);
	free(rosrpc_uri_str);

	return s;
}

service_client_t* node_service_client(node_t* n, string name, string type, bool persistent)
{
	service_client_t* s = service_create(name, type, persistent, NULL);
	if(s == NULL) return NULL;

	if(persistent)
	{
		if(!node_request_service_from_server(n, s))
		{
			service_free(s);
			return NULL;
		}
	}

	return s;
}

bool node_request_service_from_server(node_t* n, service_client_t* client)
{
	mapi_call_lookupService(n->name, client->name, response);

	uri_t* server_uri = mapi_parse_lookupService_response(response);
	if(server_uri == NULL) return FALSE;

	int socketfd = comms_connect(server_uri);

	int len;
	tcpros_send_client_request(socketfd, n->name, client->name, client->srv_type, client->persistent, response, &len);

	char callerid[128];
	tcpros_parse_server_response(response, len, callerid, NULL, NULL);

	service_add_connection(client, connection_create(server_uri, callerid, socketfd));

	return TRUE;
}

bool node_call_service(node_t* n, service_client_t* client, void* srv)
{
	if(!client->persistent)
	{
		if(!node_request_service_from_server(n, client))
		{
			return FALSE;
		}
	}

	connection_t* con = client->conns->begin->data;

	int len = client->srv_type->serialize_req(request, srv);
	len = comms_send(con->socketfd, request, len);
	if(len <= 0) return FALSE;

	len = comms_recv(con->socketfd, response, NODE_MAX_RESPONSE_LEN);
	if(len <= 0) return FALSE;

	if(response[0] == 0) return FALSE;

	client->srv_type->deserialize_resp(response + 4 + 1, srv);

	if(!client->persistent)
	{
		service_remove_connection(client, con);
	}

	return TRUE;
}

void node_spin_service_server(service_server_t* s)
{
	if(!comms_check_for_data(s->conns)) return;

	list_item_t* it;
	for(it = s->conns->begin; it != NULL; it = it->next)
	{
		connection_t* con = it->data;

		if (!con->pending) continue;

		int len = comms_recv(con->socketfd, request, NODE_MAX_RESPONSE_LEN);
	   	if(len <= 0)
	   	{
	   		PRINT("[node] Service client %s has closed connection to service %s.\n", con->name, s->name);
			service_remove_connection(s, con);
	   		return;
	   	}

	   	void* srv = s->srv_type->deserialize_req(request);
   		s->callback(srv);
		len = s->srv_type->serialize_resp(response, srv);
		s->srv_type->free_srv(srv);

	   	len = comms_send(con->socketfd, response, len);
   		if(len <= 0)
	   	{
	   		PRINT("[node] Service client %s has closed connection to service %s.\n", con->name, s->name);
			service_remove_connection(s, con);
	   		return;
	   	}
	}
}

topic_t* node_get_published_topic_by_name(node_t* n, string name)
{
	list_item_t* item = n->published_topics->begin;
	topic_t* t;

	while(item != NULL)
	{
		t = item->data;
		if(strcmp(t->name, name) == 0)
		{
			return t;
		}
	}

	return NULL;
}

topic_t* node_get_subscribed_topic_by_name(node_t* n, string name)
{
	list_item_t* item = n->subscribed_topics->begin;
	topic_t* t;

	while(item != NULL)
	{
		t = item->data;
		if(strcmp(t->name, name) == 0)
		{
			return t;
		}
	}

	return NULL;
}

service_server_t* node_get_service_server_by_name(node_t* n, string name)
{
	list_item_t* item = n->service_servers->begin;
	service_server_t* s;

	while(item != NULL)
	{
		s = item->data;
		if(strcmp(s->name, name) == 0)
		{
			return s;
		}
	}

	return NULL;
}

void node_getBusInfo(node_t* n, string request, int socketfd)
{
	sapi_respond_getBusInfo(socketfd, n->subscribed_topics, n->published_topics);
}

void node_getBusStats(node_t* n, string request, int socketfd)
{
	PRINT("[node] getBusStats is not supported in pico_ros.\n");
}

void node_getPid(node_t* n, string request, int socketfd)
{
	sapi_respond_getPid(socketfd, utils_get_pid());
}

void node_getPublications(node_t* n, string request, int socketfd)
{
	sapi_respond_getPublications_getSubscriptions(socketfd, n->published_topics);
}

void node_getSubscriptions(node_t* n, string request, int socketfd)
{
	sapi_respond_getPublications_getSubscriptions(socketfd, n->subscribed_topics);
}

void node_paramUpdate(node_t* n, string request, int socketfd)
{
	PRINT("[node] paramUpdate is not supported in pico_ros.\n");
}

void node_publisherUpdate(node_t* n, string request, int socketfd)
{
	char topic_name[NODE_MAX_TOPIC_LEN];
	list_t* l = sapi_parse_publisherUpdate_request(request, topic_name);
	topic_t* t = node_get_subscribed_topic_by_name(n, topic_name);

	list_item_t* iter_old = t->conns->begin;
	list_item_t* iter_new;
	while(iter_old != NULL)
	{
		connection_t* con = iter_old->data;
		iter_old = iter_old->next;

		// Is the old connection contained in the new publishers list
		iter_new = list_contains(l, con->uri, (LIST_EQUALS_PREDICATE)uri_equals);
		if(iter_new != NULL)
		{
			// Already have the connection
			uri_free(iter_new->data);
			list_remove_item(l, iter_new);

		}
		else
		{
			// Remove the old connection
			topic_remove_connection(t, con);
		}
	}

	node_connect_to_publishers(n, t, l);

	list_free(l, (LIST_FREE_FUNC)uri_free);

	sapi_respond_publisherUpdate(socketfd);
}

void node_requestTopic(node_t* n, string request, int socketfd)
{
	bool supported_protocol;
	sapi_parse_requestTopic_request(request, &supported_protocol);
	if(supported_protocol)
	{
		sapi_respond_requestTopic(socketfd, n->tcpros_server_uri);
	}
}

void node_getMasterUri(node_t* n, string request, int socketfd)
{
	sapi_respond_getMasterUri(socketfd, n->master_uri);
}

void node_shutdown(node_t* n, string request, int socketfd)
{
	char message[NODE_MAX_MESSAGE_LEN];
	sapi_parse_shutdown_request(request, message);

	PRINT("[node] Shutting down. %s\n", message);

	n->exit = TRUE;

	sapi_respond_shutdown(socketfd);
}

/* /////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2013, Robotic Ltd.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice, this
//     list of conditions and the following disclaimer.
//
//   * Redistributions in binary form must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation and/or
//     other materials provided with the distribution.
//
//   * Neither the name of the Robotic Ltd. nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
*/

#ifndef NODE_HEADER
#define NODE_HEADER

#include "types.h"
#include "topic.h"
#include "service.h"

typedef struct node_t
{
	string ip;
	string name;

	bool exit;

	uri_t* master_uri;

	uri_t* sapi_server_uri;
	//int sapi_server_socketfd;

	uri_t* tcpros_server_uri;
	//int tcpros_server_socketfd;

	uri_t* rosrpc_server_uri;
	//int rosrpc_server_socketfd;

	list_t* published_topics; 	/* topic_t */
	list_t* subscribed_topics; 	/* topic_t */
	list_t* service_servers; 	/* service_t */

	int server_sockets[NODE_SERVERS_COUNT];
	int client_sockets[NODE_SERVERS_COUNT];

} node_t;

extern node_t pico_node;

bool node_init(node_t* n, string node_name);
void node_stop(node_t* n);
void node_spin(node_t* n);
void node_free(node_t* n);

void node_spin_sapi_server(node_t* n);
void node_spin_tcpros_server(node_t* n);
void node_spin_rpcros_server(node_t* n);

/* Receive topic messages */
void node_spin_subscribed_topic(topic_t* t);
/* Publish topic messages */
void node_publish_topic(topic_t* t, const void* msg);

// NOTE: Could add unadvertise, unsubscribe, etc...
topic_t* node_advertise(node_t* n, string name, string type);
topic_t* node_subscribe(node_t* n, string name, string type, void (*callback)(const void*));
void node_connect_to_publishers(node_t *n, topic_t *t, list_t *pubs);

service_server_t* node_service_server(node_t* n, string name, string type, void (*callback)(void*));
service_client_t* node_service_client(node_t* n, string name, string type, bool persistent);
bool node_request_service_from_server(node_t* n, service_client_t* client);

/* Service client */
bool node_call_service(node_t* n, service_client_t* client, void* srv);
/* Service server */
void node_spin_service_server(service_server_t* s);

topic_t* node_get_published_topic_by_name(node_t* n, string name);
topic_t* node_get_subscribed_topic_by_name(node_t* n, string name);

service_server_t* node_get_service_server_by_name(node_t* n, string name);

void node_getBusInfo(node_t* n, string request, int socketfd);
void node_getBusStats(node_t* n, string request, int socketfd);
void node_getPid(node_t* n, string request, int socketfd);
void node_getPublications(node_t* n, string request, int socketfd);
void node_getSubscriptions(node_t* n, string request, int socketfd);
void node_paramUpdate(node_t* n, string request, int socketfd);
void node_publisherUpdate(node_t* n, string request, int socketfd);
void node_requestTopic(node_t* n, string request, int socketfd);
void node_getMasterUri(node_t* n, string request, int socketfd);
void node_shutdown(node_t* n, string request, int socketfd);

#endif

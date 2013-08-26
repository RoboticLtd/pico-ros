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

#ifndef SLAVE_API_HEADER
#define SLAVE_API_HEADER

#include "types.h"

// getBusInfo
void sapi_respond_getBusInfo(int client_socket, list_t* sub, list_t* pub);

// getPid
void sapi_respond_getPid(int client_socket, unsigned int pid);

// getPublicatioins / getSubscriptions
void sapi_respond_getPublications_getSubscriptions(int client_socket, list_t* pub);

// publisherUpdate
void sapi_respond_publisherUpdate(int client_socket);
list_t* sapi_parse_publisherUpdate_request(const string request, string topic);

// requestTopic
bool sapi_call_requestTopic(const uri_t *slave_uri,
                            const string caller_id,
                       		const string topic,
                       		const string protocols,
                       		string response);
void sapi_respond_requestTopic(int client_socket,
                               const uri_t *tcpros_server_uri);
void sapi_parse_requestTopic_request(const string request,
									 bool *supported_protocol);
void sapi_parse_requestTopic_response(const string response,
                                      uri_t **uri);

// getMasterUri
void sapi_respond_getMasterUri(int client_socket, uri_t* uri);

// shutdown
void sapi_respond_shutdown(int client_socket);
void sapi_parse_shutdown_request(const string request, string msg);

#endif

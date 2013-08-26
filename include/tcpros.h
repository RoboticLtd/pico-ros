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

#ifndef TCPROS_HEADER
#define TCPROS_HEADER

#include "types.h"
#include "message_type.h"
#include "service_type.h"

// Not implemented arguments: tcp_nodelay
void tcpros_send_subscriber_request(const int socketfd,
                                    const string callerid,
                                    const string topic,
                                    const message_type_t* msg,
                                    char* response,
                                    int* response_len);

void tcpros_parse_subscriber_request(const char* req,
                                     int req_len,
                                     char* callerid,
                                     char* topic,
                                     char* md5sum,
                                     char* type);


// Not implemented arguments: latching
void tcpros_send_publisher_response(const int socketfd,
                                    const string callerid,
                                    const message_type_t* msg);

void tcpros_parse_publisher_response(const char* resp,
                                     int resp_len,
                                     char* callerid,
                                     char* md5sum,
                                     char* type);


void tcpros_send_client_request(const int socketfd,
                                const string callerid,
                                const string service,
                                const service_type_t* srv,
                                const bool persistent,
                                char* response,
                                int* response_len);

void tcpros_parse_client_request(const char* req,
                                 int req_len,
                                 char* callerid,
                                 char* service,
                                 char* md5sum,
                                 char* type,
                                 bool* persistent,
                                 bool* probe);

void tcpros_send_server_response(const int socketfd,
                                 const string callerid,
                                 const service_type_t* srv);

void tcpros_parse_server_response(const char* resp,
                                  int resp_len,
                                  char* callerid,
                                  char* md5sum,
                                  char* type);

#endif

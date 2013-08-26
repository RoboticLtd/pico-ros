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

#ifndef COMMS_HEADER
#define COMMS_HEADER

#include "node.h"
#include "types.h"

__attribute__((__unused__)) static const char* request_header_prototype =
	"POST / HTTP/1.0\n"
	"Content-Type: text/xml\n"
	"Content-Length: %d\n"
	"\n";

__attribute__((__unused__)) static const char* response_header_prototype =
	"HTTP/1.1 200 OK\n"
	"Content-Type: text/xml\n"
	"Content-length: %d\n"
	"\n";

bool comms_init();
bool comms_get_own_ip(char** ip_addr, const string interface);

int comms_connect(const uri_t* server);
int comms_send(int socketfd, char* data, int data_len);
int comms_recv(int socketfd, char* data, int data_len);
void comms_disconnect(int socketfd);

int comms_init_local_server(uri_t* server);
int comms_accept_client(int socketfd);
void comms_check_for_clients(const int* servers, int* clients, const int n);
bool comms_check_for_data(list_t* conns);

#endif

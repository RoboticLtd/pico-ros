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

#ifndef TYPES_HEADER
#define TYPES_HEADER

#define TCP_MAX_PACKAGE_LEN 	2048

#define MAPI_MAX_BODY_LEN		2048
#define MAPI_MAX_HEADER_LEN		512
#define MAPI_MAX_URI_LEN		32

#define SAPI_MAX_BODY_LEN		2048
#define SAPI_MAX_HEADER_LEN		512
#define SAPI_MAX_URI_LEN		32

#define TOPIC_MAX_MESSAGE_LEN	1024

#define NODE_MAX_REQUEST_LEN	2048
#define NODE_MAX_RESPONSE_LEN 	2048
#define NODE_MAX_NAME_LEN		32
#define NODE_MAX_METHOD_NAME	64
#define NODE_MAX_TOPIC_LEN		32
#define NODE_MAX_MESSAGE_LEN	256

#define SAPI_SERVER 			0
#define TCPROS_SERVER 			1
#define ROSRPC_SERVER 			2

#define NODE_SERVERS_COUNT  	3

#define PARAM_MAX_RESPONSE_LEN	2048

#define SELECT_TIMEOUT_SEC		0
#define SELECT_TIMEOUT_USEC		1 /* 1us */

#define MIN_URI_PARSE_ITER_STEP	10 // strlen(http://_:_)

#define COMMS_MIN_PACKAGE_LEN	100

#define LIST_FREE_FUNC			void (*)(void*)
#define LIST_EQUALS_PREDICATE	bool (*)(const void*, const void*)

#include <stdint.h>
/* Boolean type */
typedef uint8_t bool;

extern const bool TRUE;
extern const bool FALSE;

/* String type */
typedef char* string;

/* uri_t */
typedef struct uri_t
{
	string ip;
	unsigned short port;
} uri_t;

uri_t* uri_create(const string _hostname, const unsigned short _port);
uri_t* uri_create_from_http_string(const string str);
uri_t* uri_create_from_rosrpc_string(const string str);
void uri_free(uri_t* uri);

string uri_get_string(uri_t* uri);
string uri_get_http_string(uri_t* uri);
string uri_get_rosrpc_string(uri_t* uri);

bool uri_equals(const uri_t* a, const uri_t* b);

/* Time structure */
typedef struct time
{
	int32_t secs;
	int32_t nsecs;
} time;

typedef time duration;

/* List structure */
typedef struct list_item_t
{
	void* data;
	struct list_item_t *next;
} list_item_t;

typedef struct list_t
{
	int size;
	struct list_item_t *begin;
	struct list_item_t *end;
} list_t;

list_t* list_create();
void list_free(list_t* l, void (free_data)(void* data));
void list_insert_item(list_t* l, void* data);
void list_remove_item(list_t* l, list_item_t* item);
list_item_t* list_contains(list_t* l, const void* data,
                           bool (equals)(const void*, const void*));

#endif

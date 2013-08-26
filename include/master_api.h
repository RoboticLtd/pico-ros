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

#ifndef MASTER_API_HEADER
#define MASTER_API_HEADER

#include "types.h"

bool mapi_call_registerPublisher(string caller_id,
		                       	 string topic,
		                       	 string topic_type,
		                       	 string caller_api,
		                       	 string response);

bool mapi_call_unregisterPublisher(string caller_id,
		                       	   string topic,
		                       	   string caller_api,
		                       	   string response);

bool mapi_call_registerSubscriber(string caller_id,
			                      string topic,
			                      string topic_type,
			                      string caller_api,
			                      string response);

bool mapi_call_unregisterSubscriber(string caller_id,
		                       	    string topic,
		                       	    string caller_api,
		                       	    string response);

list_t* mapi_parse_registerSubscriber_response(const string response);

bool mapi_call_registerService(string caller_id,
                               string service,
                               string service_api,
                               string caller_api,
                               string response);

bool mapi_call_unregisterService(string caller_id,
		                       	 string service,
		                       	 string caller_api,
		                       	 string response);

bool mapi_call_lookupService(string caller_id,
                             string service,
                             string response);

uri_t* mapi_parse_lookupService_response(const string response);

// Parameter server
bool mapi_call_setParam(const string caller_id,
     	                const string param_name,
        	            const string param_tag,
            	        const string param_value,
            	        string response);

bool mapi_call_getParam(const string caller_id,
     	                const string param_name,
            	        string response);

string mapi_parse_getParam_response(const string response);

bool mapi_call_hasParam(const string caller_id,
     	                const string param_name,
            	        string response);

bool mapi_call_deleteParam(const string caller_id,
     	                   const string param_name,
            	           string response);

bool mapi_parse_deleteParam_response(const string response);

#endif

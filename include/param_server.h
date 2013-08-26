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

#ifndef PARAM_SERVER_HEADER
#define PARAM_SERVER_HEADER

/*32-bit integers
booleans
strings
doubles*/

#include <stdint.h>

#include "types.h"

/*#define param_server_set(n, p) _Generic((p), \
	int32_t*: param_server_set_int32, \
	double*: param_server_set_double, \
	bool*: param_server_set_bool, \
	char*: param_server_set_string, \
    default: param_server_unsupported_type \
    )(n, p)
void param_server_unsupported_type(string name, void* param);
*/

void param_server_set_int32(const string name, const int32_t param);
void param_server_set_double(const string name, const double param);
void param_server_set_bool(const string name, const bool param);
void param_server_set_string(const string name, const string param);


bool param_server_get_int32(const string name, int32_t* res);
bool param_server_get_double(const string name, double* res);
bool param_server_get_bool(const string name, bool* res);
bool param_server_get_string(const string name, string res);

bool param_server_delete_param(const string name);





#endif

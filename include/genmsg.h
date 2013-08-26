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

#ifndef XMLGEN_HEADER
#define XMLGEN_HEADER

#include "types.h"
#include "topic.h"

inline void xmlmsg_init(string msg);
inline void xmlmsg_open_tag(string msg, string tag);
inline void xmlmsg_add_tag_with_string(string msg, string tag, string value);
inline void xmlmsg_add_tag_with_ushort(string msg, string tag, unsigned short value);
inline void xmlmsg_add_tag_with_uint(string msg, string tag, unsigned int value);
inline void xmlmsg_close_tag(string msg, string tag);

inline void xmlmsg_open_method_response(string msg);
inline void xmlmsg_close_method_response(string msg);
inline void xmlmsg_add_topic(string msg, topic_t* t);

inline void tcpmsg_init(char **buf);
inline void tcpmsg_add_field(char **buf, const string name, const char* value);
inline void tcpmsg_add_data(char **buf, const void* data, int data_len);
inline void tcpmsg_add_raw_data(char **buf, const void* data, int data_len);
inline void tcpmsg_finalize(char *buf, int len);

#endif

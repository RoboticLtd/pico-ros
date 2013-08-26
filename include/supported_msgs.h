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

#ifndef SUPPORTED_MSGS_HEADER
#define SUPPORTED_MSGS_HEADER

#include "message_type.h"

#ifdef STD_MSGS__STRING
#include "std_msgs__string.h"
#else
#define STD_MSGS__STRING 0
#endif

#ifdef STD_MSGS__BYTE
#include "std_msgs__byte.h"
#else
#define STD_MSGS__BYTE 0
#endif

#define SUPPORTED_MESSAGES_COUNT \
	( STD_MSGS__STRING + \
	  STD_MSGS__BYTE )

static const message_type_t supported_msgs[] =
{
#if STD_MSGS__STRING
	{
		"std_msgs/String",
	 	"992ce8a1687cec8c8bd883ec73ca41d1",
	 	serialize_std_msgs__string,
	 	deserialize_std_msgs__string,
	 	free_std_msgs__string
	}
#endif
#if STD_MSGS__BYTE
	,{
		"std_msgs/Byte",
		"ad736a2e8818154c487bb80fe42ce43b",
		serialize_std_msgs__byte,
		deserialize_std_msgs__byte,
		free_std_msgs__byte
	}
#endif
};

#endif

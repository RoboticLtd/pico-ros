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

#ifndef PICO_ROS_HEADER
#define PICO_ROS_HEADER

#include "topic.h"
#include "service.h"
#include "param_server.h"


bool pico_ros_init(string node_name);
void pico_ros_request_shutdown();
void pico_ros_shutdown();

inline void pico_ros_spin();
inline bool pico_ros_ok();

// Publisher
inline topic_t* pico_ros_advertise(string name, string type);
inline void pico_ros_publish_topic(topic_t* t, const void* msg);

// Subscriber
#define pico_ros_subscribe(n, t, c) _pico_ros_subscribe(n, t, (msg_cb)c);
inline topic_t* _pico_ros_subscribe(string name,
                                    string type,
                                    void (*callback)(const void*));

//Service server
#define pico_ros_service_server(n, t, c) _pico_ros_service_server(n, t, (srv_cb)c);
inline service_server_t* _pico_ros_service_server(string name,
                                                  string type,
                                                  void (*callback)(void*));

// Service client
inline service_client_t* pico_ros_service_client(string name,
                                                 string type,
                                                 bool persistent);

inline bool pico_ros_call_service(service_client_t* client, void* srv);



#endif



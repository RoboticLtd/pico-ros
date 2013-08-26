#include "pico_ros.h"

#include "node.h"
#include "comms.h"
#include "types.h"

bool pico_ros_init(string node_name)
{
	bool res = TRUE;

	res &= comms_init();
	res &= node_init(&pico_node, node_name);


	// TODO: Add rosgraph_msgs/Log to supported_msgs
	// topic_t* rosout = node_advertise(&pico_node,
	// 								  "/rosout",
	// 								  "rosgraph_msgs/Log");

	return res;
}

void pico_ros_spin()
{
	node_spin(&pico_node);
}

bool pico_ros_ok()
{
	return !pico_node.exit;
}

void pico_ros_request_shutdown()
{
	pico_node.exit = TRUE;
}

void pico_ros_shutdown()
{
	node_stop(&pico_node);
}

// Publisher
topic_t* pico_ros_advertise(string name, string type)
{
	return node_advertise(&pico_node, name, type);
}

void pico_ros_publish_topic(topic_t* t, const void* msg)
{
	node_publish_topic(t, msg);
}

// Subscriber
topic_t* _pico_ros_subscribe(string name, string type, void (*callback)(const void*))
{
	return node_subscribe(&pico_node, name, type, callback);
}

//Service server
service_server_t* _pico_ros_service_server(string name, string type, void (*callback)(void*))
{
	return node_service_server(&pico_node, name, type, callback);
}

// Service client
service_client_t* pico_ros_service_client(string name, string type, bool persistent)
{
	return node_service_client(&pico_node, name, type, persistent);
}

bool pico_ros_call_service(service_client_t* client, void* srv)
{
	return node_call_service(&pico_node, client, srv);
}


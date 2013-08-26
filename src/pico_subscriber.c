#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "pico_ros.h"
#include "std_msgs__string.h"

void listener_callback(const std_msgs__string* msg)
{
	printf("I heard: [%s]\n", msg->data);
}


void sigint_handler(int sig)
{
	pico_ros_request_shutdown();
}

int main(int argc, char const *argv[])
{
	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);

	if(!pico_ros_init("/pico_subscriber"))
	{
		printf("Unable to initialise node.\n");
		return 1;
	}

	printf("Subscribing to /chatter \n");
	pico_ros_subscribe("/chatter", "std_msgs/String", listener_callback);

	printf("Spinning node... \n");

	while(pico_ros_ok())
	{
		pico_ros_spin();
		usleep(1000 * 10);
	}

	printf("\rShutting down.\n");
	pico_ros_shutdown();

	return 0;
}

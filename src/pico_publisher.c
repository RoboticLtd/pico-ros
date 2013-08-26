#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "pico_ros.h"
#include "std_msgs__string.h"

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

	if(!pico_ros_init("/pico_publisher"))
	{
		printf("Unable to initialise node.\n");
		return 1;
	}

	topic_t* chatter = pico_ros_advertise("/chatter", "std_msgs/String");

	std_msgs__string msg;
	msg.data = (string)calloc(sizeof(char), 32);
	unsigned long msg_id = 0;

	printf("Spinning node... \n");
	while(pico_ros_ok())
	{
		sprintf(msg.data, "Hello %lu", msg_id++);

		pico_ros_publish_topic(chatter, &msg);

		pico_ros_spin();

		usleep(1000 * 10);
	}

	printf("\rShutting down.\n");
	//pico_ros_shutdown();

	return 0;
}

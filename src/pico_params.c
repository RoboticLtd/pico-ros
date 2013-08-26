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

	if(!pico_ros_init("/pico_param"))
	{
		printf("Unable to initialise node.\n");
		return 1;
	}

	param_server_set_double("pico_param", 3.1415);

	double p;
	bool b = param_server_get_double("pico_param", &p);

	printf("pico_param = %lf, %hhd\n", p, b);
	p = 0;
	param_server_delete_param("pico_param");
	b = param_server_get_double("pico_param", &p);

	printf("pico_param = %lf, %hhd\n", p, b);

	printf("\rShutting down.\n");
	pico_ros_shutdown();

	return 0;
}

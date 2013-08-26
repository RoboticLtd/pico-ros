#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "pico_ros.h"
#include "roscpp_tutorials__TwoInts.h"

void server_callback(roscpp_tutorials__TwoInts* srv)
{
	printf("Request: %ld + %ld\n", srv->request.x, srv->request.y);
	srv->response.sum = srv->request.x + srv->request.y;
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

	if(!pico_ros_init("/pico_server"))
	{
		printf("Unable to initialise node.\n");
		return 1;
	}

	printf("Advertising /add_two_ints \n");

	pico_ros_service_server("/add_two_ints",
                        	"roscpp_tutorials/TwoInts",
                        	server_callback);

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

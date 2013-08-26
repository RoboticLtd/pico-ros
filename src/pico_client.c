#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "pico_ros.h"
#include "roscpp_tutorials__TwoInts.h"

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

	if(!pico_ros_init("/pico_client"))
	{
		printf("Unable to initialise node.\n");
		return 1;
	}

	printf("Initialising service client\n");
	service_client_t* client = pico_ros_service_client(
	                                        "/add_two_ints",
	                                        "roscpp_tutorials/TwoInts",
	                                        TRUE);
	if(client == NULL)
	{
		printf("Unable to create client.\n");
		return 1;
	}
	printf("Initialised service client\n");
	roscpp_tutorials__TwoInts srv;
	srv.request.x = 199;
	srv.request.y = 199;

	printf("Spinning node... \n");
	while(pico_ros_ok())
	{

		if(pico_ros_call_service(client, &srv))
		{
			printf("Result: %ld\n", srv.response.sum);
		}

		usleep(1000 * 10);

		pico_ros_spin();
	}

	printf("\rShutting down.\n");
	pico_ros_shutdown();

	return 0;
}

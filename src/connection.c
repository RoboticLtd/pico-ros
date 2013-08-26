#include "connection.h"

#include <stdlib.h>
#include <string.h>

#include "comms.h"

connection_t* connection_create(uri_t* _uri, string _name, int _socketfd)
{
	connection_t* con = (connection_t*)malloc(sizeof(connection_t));

	/* Set the SAPI uri */
	con->uri = (_uri != NULL) ? uri_create(_uri->ip, _uri->port) : NULL;

	/* Set the name of the topic */
	con->name =  (char*)calloc(sizeof(char), strlen(_name) + 1);
	strcpy(con->name, _name);

	/* Set the socket */
	con->socketfd = _socketfd;

	con->pending = FALSE;

	return con;
}

void connection_free(connection_t* con)
{
	comms_disconnect(con->socketfd);
	if(con->uri != NULL) uri_free(con->uri);
	if(con->name != NULL) free(con->name);
	free(con);
}

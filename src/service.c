#include "service.h"

#include <stdlib.h>
#include <string.h>

#include "comms.h"

service_t* service_create(const string _name,
                          const string _type,
                          bool _persistent,
                          void (*_callback)(void*))
{
	service_t* s = (service_t*)malloc(sizeof(service_t));

	/* Set the name of the service */
	s->name = (char*)calloc(sizeof(char), strlen(_name) + 1);
	strcpy(s->name, _name);

	/* Set the service type */
	s->srv_type = services_get_type(_type);

	/* Check if the message is supported */
	if(s->srv_type == NULL)
	{
		service_free(s);
		return NULL;
	}

	s->callback = _callback;

	s->persistent = _persistent;

	s->conns = list_create();

	return s;
}

void service_free(service_t* s)
{
	if(s->name == NULL) free(s->name);
	if(s->conns == NULL) list_free(s->conns, (LIST_FREE_FUNC)connection_free);
	free(s);
}

void service_add_connection(service_t* s, connection_t* con)
{
	list_insert_item(s->conns, con);
}

void service_remove_connection(service_t* t, connection_t* con)
{
	comms_disconnect(con->socketfd);

	list_item_t* item = t->conns->begin;

	while(item != NULL)
	{
		if(item->data == con)
		{
			connection_free(con);
			list_remove_item(t->conns, item);
			return;
		}

		item = item->next;
	}
}

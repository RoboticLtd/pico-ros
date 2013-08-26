
#include "topic.h"

#include <stdlib.h>
#include <string.h>

#include "comms.h"

topic_t* topic_create(const string _name,
                	  direction_t _dir,
                	  const string _type,
                	  void (*_callback)(const void*))
{
	topic_t* t = (topic_t*)malloc(sizeof(topic_t));

	/* Set the name of the topic */
	t->name = (char*)calloc(sizeof(char), strlen(_name) + 1);
	strcpy(t->name, _name);

	/* Set the direction of the topic */
	t->dir = _dir;

	/* Set the message type of the topic */
	t->msg_type = message_get_type(_type);

	/* Check if the message is supported */
	if(t->msg_type == NULL)
	{
		topic_free(t);
		return NULL;
	}

	if(t->dir == INBOUND)
	{
		t->callback = _callback;
	}

	t->conns = list_create();

	return t;
}

void topic_free(topic_t* t)
{
	if(t->name == NULL) free(t->name);
	if(t->conns == NULL) list_free(t->conns, (LIST_FREE_FUNC)connection_free);
	free(t);
}

connection_t* topic_get_connection_by_name(topic_t* t, string name)
{
	list_item_t* item = t->conns->begin;
	connection_t* con;

	while(item != NULL)
	{
		con = item->data;

		if(strcmp(con->name, name) == 0)
		{
			return con;
		}

		item = item->next;
	}

	return NULL;
}

void topic_add_connection(topic_t* t, connection_t* con)
{
	list_insert_item(t->conns, con);
}

void topic_remove_connection(topic_t* t, connection_t* con)
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

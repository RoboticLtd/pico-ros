#include "genmsg.h"

#include <stdio.h>
#include <string.h>

#include "utils.h"

void xmlmsg_init(string msg)
{
	SPRINT(msg, "<?xml version=\"1.0\"?>\n");
}

void xmlmsg_open_tag(string msg, string tag)
{
	SPRINT(msg + strlen(msg), "<%s>\n", tag);
}

void xmlmsg_add_tag_with_string(string msg, string tag, string value)
{
	SPRINT(msg + strlen(msg), "<%s>%s</%s>\n", tag, value, tag);
}

void xmlmsg_add_tag_with_ushort(string msg, string tag, unsigned short value)
{
	SPRINT(msg + strlen(msg), "<%s>%hu</%s>\n", tag, value, tag);
}

void xmlmsg_add_tag_with_uint(string msg, string tag, unsigned int value)
{
	SPRINT(msg + strlen(msg), "<%s>%u</%s>\n", tag, value, tag);
}

void xmlmsg_close_tag(string msg, string tag)
{
	SPRINT(msg + strlen(msg), "</%s>\n", tag);
}

void xmlmsg_open_method_response(string msg)
{
	xmlmsg_open_tag(msg, "methodResponse");
		xmlmsg_open_tag(msg, "params");
			xmlmsg_open_tag(msg, "param");
				xmlmsg_open_tag(msg, "value");
					xmlmsg_open_tag(msg, "array");
						xmlmsg_open_tag(msg, "data");
}

void xmlmsg_close_method_response(string msg)
{
						xmlmsg_close_tag(msg, "data");
					xmlmsg_close_tag(msg, "array");
				xmlmsg_close_tag(msg, "value");
			xmlmsg_close_tag(msg, "param");
		xmlmsg_close_tag(msg, "params");
	xmlmsg_close_tag(msg, "methodResponse");
}

void xmlmsg_add_topic(string msg, topic_t* t)
{
	list_item_t* con_it;
	for(con_it = t->conns->begin; con_it != NULL; con_it = con_it->next)
	{
		connection_t* con = con_it->data;
		int id = 0xFFFFFFFF & (unsigned long)con;
		xmlmsg_open_tag(msg, "value");
			xmlmsg_open_tag(msg, "array");
				xmlmsg_open_tag(msg, "data");
					xmlmsg_open_tag(msg, "value");
						xmlmsg_add_tag_with_uint(msg, "i4", id);
					xmlmsg_close_tag(msg, "value");
					xmlmsg_add_tag_with_string(msg, "value", con->name);
					xmlmsg_add_tag_with_string(msg, "value", (t->dir == INBOUND) ? "i" : "o");
					xmlmsg_add_tag_with_string(msg, "value", "TCPROS");
					xmlmsg_add_tag_with_string(msg, "value", t->name);
				xmlmsg_close_tag(msg, "data");
			xmlmsg_close_tag(msg, "array");
		xmlmsg_close_tag(msg, "value");
	}
}


void tcpmsg_init(char **buf)
{
	*buf += 4;
}

void tcpmsg_add_field(char **buf, string name, const char* value)
{
	int field_len = strlen(name) + strlen(value) + 1; /* Count the = sign as well */

	memcpy(*buf,(char*)&field_len, 4);
	SPRINT(*buf + 4,"%s=%s", name, value);

	*buf += field_len + 4;
}

void tcpmsg_add_data(char **buf, const void* data, int data_len)
{
	memcpy(*buf,(char*)&data_len, 4);
	memcpy(*buf + 4, data, data_len);

	*buf += data_len + 4;
}

void tcpmsg_add_raw_data(char **buf, const void* data, int data_len)
{
	memcpy(*buf, data, data_len);

	*buf += data_len;
}

void tcpmsg_finalize(char *buf, int buf_len)
{
	int data_len = buf_len - 4; /* Ignore package len field itself */
	memcpy(buf,(char*)&data_len, 4);
}

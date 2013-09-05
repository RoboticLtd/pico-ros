#ifdef STD_MSGS__STRING
#include "std_msgs__string.h"

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "genmsg.h"

int serialize_std_msgs__string(char* const buf, const void* msg)
{
	char* data = ((std_msgs__string*)msg)->data;
	uint32_t len = strlen(data);

	char* iter = buf;
	tcpmsg_init(&iter);
	tcpmsg_add_string(&iter, data, len);
	tcpmsg_finalize(buf, iter - buf);

	return iter-buf;
}

void* deserialize_std_msgs__string(char* const buf)
{
	char* iter = buf;

	//Read the package len
	//int package_len = utils_get_int_from_bytes(iter, sizeof(int32_t));
	iter += sizeof(int32_t);

	std_msgs__string* msg =
		(std_msgs__string*)malloc(sizeof(std_msgs__string));

	// Read the length of the field
	int field_len = utils_get_int_from_bytes(iter, sizeof(int32_t));
	iter +=  sizeof(int32_t);



	msg->data = (char*)calloc(sizeof(char), field_len + 1);

	memcpy(msg->data, iter, field_len);
	msg->data[field_len] = '\0';

	return msg;
}

void free_std_msgs__string(void* msg)
{
	if(((std_msgs__string*)msg)->data != NULL)
		free(((std_msgs__string*)msg)->data);

	if(msg != NULL) free(msg);
}

#endif

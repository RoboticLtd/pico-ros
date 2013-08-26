#ifdef STD_MSGS__BYTE
#include "std_msgs__byte.h"

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "genmsg.h"

int serialize_std_msgs__byte(char* const buf, const void* msg)
{
	const uint8_t data = ((std_msgs__byte*)msg)->data;
	char* iter = buf;
	tcpmsg_init(&iter);
	tcpmsg_add_byte(&iter, &data);
	tcpmsg_finalize(buf, iter - buf);
	return iter-buf;
}

void* deserialize_std_msgs__byte(char* const buf)
{
	char* iter = buf;

	//Read the package len
	//int package_len = utils_get_int_from_bytes(iter, sizeof(int32_t));
	iter += sizeof(int32_t);

	// Read the length of the field
	//int field_len = utils_get_int_from_bytes(iter, sizeof(int32_t));
	iter +=  sizeof(int32_t);

	std_msgs__byte* msg =
		(std_msgs__byte*)malloc(sizeof(std_msgs__byte));

	memcpy(&msg->data, buf, sizeof(uint8_t));
	return msg;
}

void free_std_msgs__byte(void* msg)
{
	if(msg != NULL) free(msg);
}

#endif

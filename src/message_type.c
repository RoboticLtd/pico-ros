#include "message_type.h"

#include <stdlib.h>
#include <string.h>

#include "genmsg.h"
#include "supported_msgs.h"


const message_type_t* message_get_type(string type)
{
	int i;
	for(i = 0; i < SUPPORTED_MESSAGES_COUNT; ++i)
	{
		if(strcmp(supported_msgs[i].type, type) == 0)
		{
			return supported_msgs + i;
		}
	}

	/* Message not supported */
	return NULL;
}

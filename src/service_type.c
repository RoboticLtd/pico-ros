#include "service_type.h"

#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "genmsg.h"

#include "supported_srvs.h"

const service_type_t* services_get_type(string type)
{
	int i;
	for(i = 0; i < SUPPORTED_SERVICES_COUNT; ++i)
	{
		if(strcmp(supported_srvs[i].type, type) == 0)
		{
			return supported_srvs + i;
		}
	}

	/* Message not supported */
	return NULL;
}


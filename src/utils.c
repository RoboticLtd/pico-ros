#include "utils.h"

#include <unistd.h>

int32_t utils_get_int_from_bytes(const char* bytes, int nbytes)
{
	int32_t res = 0, i;
	if(nbytes == 4)
	{
		res = (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
	}
	else
	{
		for(i = 0; i < nbytes; ++i)
		{
			res |= bytes[i] << (i * 8);
		}
	}
	return res;
}

int64_t utils_get_long_from_bytes(const char* bytes, int nbytes)
{
	int64_t res = 0l;
	int i;

	if(nbytes == 8)
	{
		res = ((int64_t)(0xff & bytes[7]) << 56) |
		 	  ((int64_t)(0xff & bytes[6]) << 48) |
		 	  ((int64_t)(0xff & bytes[5]) << 40) |
		 	  ((int64_t)(0xff & bytes[4]) << 32) |
			  ((int64_t)(0xff & bytes[3]) << 24) |
		 	  ((int64_t)(0xff & bytes[2]) << 16) |
		 	  ((int64_t)(0xff & bytes[1]) << 8) |
		 	   (int64_t)(0xff & bytes[0]);
	}
	else
	{
		for(i = 0; i < nbytes; ++i)
		{
			res |= (int64_t)(0xff & bytes[i]) << (i * 8);
		}
	}

	return res;
}

int utils_get_pid()
{
	return getpid();
}

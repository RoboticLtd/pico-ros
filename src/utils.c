#include "utils.h"

#include <unistd.h>

int utils_get_int_from_bytes(const char* bytes, int nbytes)
{
	int res = 0, i;
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

long utils_get_long_from_bytes(const char* bytes, int nbytes)
{
	long res = 0l;
	int i;

	if(nbytes == 8)
	{
		res = ((long)(0xff & bytes[7]) << 56) |
		 	  ((long)(0xff & bytes[6]) << 48) |
		 	  ((long)(0xff & bytes[5]) << 40) |
		 	  ((long)(0xff & bytes[4]) << 32) |
			  ((long)(0xff & bytes[3]) << 24) |
		 	  ((long)(0xff & bytes[2]) << 16) |
		 	  ((long)(0xff & bytes[1]) << 8) |
		 	  (long)(0xff & bytes[0]);
	}
	else
	{
		for(i = 0; i < nbytes; ++i)
		{
			res |= (long)(0xff & bytes[i]) << (i * 8);
		}
	}

	return (long)res;
}

int utils_get_pid()
{
	return getpid();
}

#ifdef ROSCPP_TUTORIALS__TWOINTS
#include "roscpp_tutorials__TwoInts.h"

#include <stdlib.h>

#include "utils.h"

int serialize_req_roscpp_tutorials__TwoInts(char* const buf, const void* srv)
{
	const roscpp_tutorials__TwoInts* data = srv;
	char* iter = buf;

	tcpmsg_init(&iter);
	tcpmsg_add_raw_data(&iter, (char*)(&data->request.x), 8);
	tcpmsg_add_raw_data(&iter, (char*)(&data->request.y), 8);
	tcpmsg_finalize(buf, iter - buf);
	return iter-buf;
}

void* deserialize_req_roscpp_tutorials__TwoInts(char* const buf)
{
	roscpp_tutorials__TwoInts* srv =
		(roscpp_tutorials__TwoInts*)malloc(sizeof(roscpp_tutorials__TwoInts));

	char* iter = buf;

	//Read the package len
	//int package_len = utils_get_int_from_bytes(iter, sizeof(int32_t));
	iter += sizeof(int32_t);

	srv->request.x = utils_get_long_from_bytes(iter, sizeof(int64_t));
	iter += sizeof(int64_t);

	srv->request.y = utils_get_long_from_bytes(iter, sizeof(int64_t));
	iter += sizeof(int64_t);

	return srv;
}

int serialize_resp_roscpp_tutorials__TwoInts(char* const buf, const void* srv)
{
	const roscpp_tutorials__TwoInts* data = srv;
	buf[0] = 1;
	char* iter = buf + 1;
	tcpmsg_add_int64(&iter, (char*)(&data->response.sum));
	return iter - buf;
}

void deserialize_resp_roscpp_tutorials__TwoInts(char* const buf, void* srv)
{
	roscpp_tutorials__TwoInts* data = srv;
	char* iter = buf;
	data->response.sum = utils_get_long_from_bytes(iter, 8);
}

void free_roscpp_tutorials__TwoInts(void* srv)
{
	if(srv != NULL) free(srv);
}

#endif

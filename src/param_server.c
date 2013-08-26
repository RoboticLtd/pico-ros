#include "param_server.h"

#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "utils.h"
#include "master_api.h"


char response[PARAM_MAX_RESPONSE_LEN];

void param_server_set_int32(const string name, const int32_t param)
{
	char value[16];
	SPRINT(value, "%d", param);
	mapi_call_setParam(pico_node.name, name, "int", value, response);
}

void param_server_set_double(const string name, const double param)
{
	char value[32];
	SPRINT(value, "%.16lf", param);
	mapi_call_setParam(pico_node.name, name, "double", value, response);
}

void param_server_set_bool(const string name, const bool param)
{
	char* value = (param) ? "1" : "0";
	mapi_call_setParam(pico_node.name, name, "boolean", value, response);
}

void param_server_set_string(const string name, const string param)
{
	mapi_call_setParam(pico_node.name, name, "string", param, response);
}

bool param_server_get_int32(const string name, int32_t* res)
{
	mapi_call_getParam(pico_node.name, name, response);
	string value = mapi_parse_getParam_response(response);
	if(value == NULL) return FALSE;

	SSCAN(value, "<int>%d", res);
	return TRUE;
}

bool param_server_get_double(const string name, double* res)
{
	mapi_call_getParam(pico_node.name, name, response);
	string value = mapi_parse_getParam_response(response);
	if(value == NULL) return FALSE;

	SSCAN(value, "<double>%lf", res);
	return TRUE;
}

bool param_server_get_bool(const string name, bool* res)
{
	mapi_call_getParam(pico_node.name, name, response);
	string value = mapi_parse_getParam_response(response);
	if(value == NULL) return FALSE;

	SSCAN(value, "<boolean>%hhd", res);
	return TRUE;
}

bool param_server_get_string(const string name, string res)
{
	mapi_call_getParam(pico_node.name, name, response);
	string value = mapi_parse_getParam_response(response);
	if(value == NULL) return FALSE;

	int res_len = strlen(value) - 16; // - strlen(<string></string>) + strlen("\0")
	if(res == NULL) res = (string)calloc(sizeof(char), res_len);

	strncpy(res, value + 8, res_len - 1);
	return TRUE;
}

bool param_server_delete_param(const string name)
{
	return mapi_call_deleteParam(pico_node.name, name, response) ?
		   mapi_parse_deleteParam_response(response) :
		   FALSE;
}

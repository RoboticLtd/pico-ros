#include "types.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "utils.h"

/* Boolean */
const bool TRUE = 1;
const bool FALSE = 0;

/*  Uri */
#define MAX_URI_STRING_LEN 	64
#define MAX_PORT_STRING_LEN	6

uri_t* uri_create(const string _hostname, const unsigned short _port)
{
	uri_t* uri;
	uri = (uri_t*)malloc(sizeof(uri_t));
	uri->ip = (string)calloc(sizeof(char*), MAX_URI_STRING_LEN);

	strcpy(uri->ip, _hostname);
	struct hostent *lh = gethostbyname(uri->ip);
	PRINT("Resolved addr: %d \r\n", lh->h_addr);
	inet_ntop(AF_INET, lh->h_addr, uri->ip, MAX_URI_STRING_LEN);

	uri->port = _port;

	return uri;
}

uri_t* uri_create_from_http_string(const string str)
{
	char* iter = strstr(str, "http://");
	if(iter == NULL) return NULL;

	uri_t* uri;
	uri = (uri_t*)malloc(sizeof(uri_t));
	uri->ip = (string)calloc(sizeof(char*), MAX_URI_STRING_LEN);

	SSCAN(iter, "http://%[^:]:%hu", uri->ip, &(uri->port));

	struct hostent *lh = gethostbyname(uri->ip);
	inet_ntop(AF_INET, lh->h_addr, uri->ip, MAPI_MAX_URI_LEN);

	return uri;
}

uri_t* uri_create_from_rosrpc_string(const string str)
{
	char* iter = strstr(str, "rosrpc://");
	if(iter == NULL) return NULL;

	uri_t* uri;
	uri = (uri_t*)malloc(sizeof(uri_t));
	uri->ip = (string)calloc(sizeof(char*), MAX_URI_STRING_LEN);

	SSCAN(iter, "rosrpc://%[^:]:%hu", uri->ip, &(uri->port));

	struct hostent *lh = gethostbyname(uri->ip);
	inet_ntop(AF_INET, lh->h_addr, uri->ip, MAPI_MAX_URI_LEN);

	return uri;
}

void uri_free(uri_t* uri)
{
	if(uri->ip != NULL) free(uri->ip);
	free(uri);
}

string uri_get_string(uri_t* uri)
{
	string res = (char*)calloc(sizeof(char), MAX_URI_STRING_LEN);
	SPRINT(res, "%s:%u", uri->ip, uri->port);
	return res;
}

string uri_get_http_string(uri_t* uri)
{
	string res = (char*)calloc(sizeof(char), MAX_URI_STRING_LEN);
	SPRINT(res, "http://%s:%u/", uri->ip, uri->port);
	return res;
}

string uri_get_rosrpc_string(uri_t* uri)
{
	string res = (char*)calloc(sizeof(char), MAX_URI_STRING_LEN);
	SPRINT(res, "rosrpc://%s:%u/", uri->ip, uri->port);
	return res;
}

bool uri_equals(const uri_t* a, const uri_t* b)
{
	return (strcmp(a->ip, b->ip) == 0 && (a->port == b->port));
}

/* List */
list_t* list_create()
{
	list_t* l = (list_t*)malloc(sizeof(list_t));
	l->size = 0;
	l->begin = NULL;
	l->end = NULL;

	return l;
}

void list_free(list_t* l, void (free_data)(void* data))
{
	list_item_t* item = l->begin;
	list_item_t* next;
	while(item != NULL)
	{
		if(item->data != NULL)
		{
			free_data(item->data);
		}

		next = item->next;
		free(item);
		item = next;
	}

	free(l);
}

void list_insert_item(list_t* l, void* data)
{
	list_item_t* item = (list_item_t*)malloc(sizeof(item));
	item->data = data;
	item->next = NULL;

	if(l->size == 0)
	{
		l->begin = l->end = item;
	}
	else
	{
		l->end->next = item;
		l->end = item;
	}

	l->size++;
}

void list_remove_item(list_t* l, list_item_t* item)
{
	list_item_t* prev = NULL;
	list_item_t* iter = l->begin;

	if(item == l->begin)
	{
		l->begin = item->next;
		free(item);
		l->size--;
		return;
	}

	while(iter != NULL)
	{
		if(iter == item)
		{
			if(iter == l->end)
			{
				l->end = prev;
				l->end->next = NULL;
			}
			else
			{
				prev->next = item->next;
			}

			free(item);
			l->size--;
			return;
		}
		else
		{
			prev = iter;
			iter = iter->next;
		}
	}
}

list_item_t* list_contains(list_t* l, const void* data,
                           bool (equals)(const void*, const void*))
{
	list_item_t* iter;
	for(iter = l->begin; iter != NULL; iter = iter->next)
	{
		if(equals(data, iter->data)) return iter;
	}

	return NULL;
}

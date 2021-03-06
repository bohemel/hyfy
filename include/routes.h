#ifndef _ROUTES_H_
#define _ROUTES_H_

#include "request.h"

struct route {
	char *request_string;
	enum RouteType {RT_STATIC, RT_DYNAMIC} type;
	char *content_type;
	char *static_content_source;
	char *static_content;
	void (*callback)(struct http_request*);
};

void routes_init();
int routes_match(struct http_request*, struct route**);
int routes_load_static_file(const char*, char**);

#endif

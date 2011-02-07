#ifndef _HYFY_H_
#define _HYFY_H_

#include "request.h"

void hyfy_log(const char*);
void hyfy_error(const char*);

void dispatch_request(int);
void destroy_request(struct http_request*);

#endif


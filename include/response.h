#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include "request.h"

void http_response_send(const char*, const char*, struct http_request*, const char*);

#endif

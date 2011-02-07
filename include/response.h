#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include "request.h"

const int MAX_OUTBUFFER_SIZE = 1024; // 1K of headerbuffer

void http_response_send(const char *, struct http_request*, const char*);

#endif

#ifndef _REQUEST_H_
#define _REQUEST_H_

#define MAX_REQUEST_PATH_LENGTH  255

struct http_request {
	enum HttpMethod {METHOD_GET, METHOD_POST} method;
	char request_path[MAX_REQUEST_PATH_LENGTH];
	int socket_fd;
};

int parse_http_request(char*, struct http_request*);

#endif

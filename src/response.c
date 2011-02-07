#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "response.h"
#include "request.h"

const int MAX_OUTBUFFER_SIZE = 1024; // 1K of headerbuffer

void http_response_send(const char *resp_code, struct http_request *request, const char *body) {
	char header_buffer[MAX_OUTBUFFER_SIZE];
	header_buffer[0] = 0;	

	int body_len = strlen(body);

	// build http response
 	strcat(header_buffer, "HTTP/1.0 ");
	strcat(header_buffer, resp_code);
	strcat(header_buffer, "\r\n");

	// append headers
	strcat(header_buffer, "Content-Type: text/html\r\n");
	

	// append extra crlf to indicate start of body
	strcat(header_buffer, "\r\n");

	
	write(request->socket_fd, header_buffer, strlen(header_buffer));
	write(request->socket_fd, body, body_len);


	return;
}

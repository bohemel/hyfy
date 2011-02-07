#include <stdio.h>
#include <string.h>

#include "request.h"
#include "hyfy.h"

int parse_http_request(char *headers, struct http_request *request) {
	if(strncmp(headers, "GET", 3) == 0) {
		request->method = METHOD_GET;
		// start counter att index 4 GET /path/to/save  
		for(int i = 4; ; i++) {
			if(strncmp((char*)&headers[i], " ", 1) == 0) {
				// terminate buffer
				request->request_path[i-4] = 0;	
				printf("Log: Found path \"%s\"\n", request->request_path);
				break;
			}
			request->request_path[i-4] = headers[i];
		}
	}
	else
		return 0;
	return 1;
}

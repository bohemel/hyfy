#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "request.h"
#include "hyfy.h"

int parse_http_request(char *headers, struct http_request *request) {
	int in_path = 0, in_query_string = 0, q_counter = 0;

	if(strncmp(headers, "GET", 3) == 0) {
		// init vars
		request->method = METHOD_GET;
		request->path[0] = 0;
		request->query[0] = 0;


		// start counter att index 4 GET /path/to/save
		char *unparsed_query = &headers[4];
		in_path = 1;
		for(int i = 0; ; i++) {
			if(unparsed_query[i] == ' ') {
				// terminate buffer
				if(in_path)	
					request->path[i] = 0;
				if(in_query_string)
					request->query[q_counter] = 0;

				// quit loop
				break;
			}
			else if(unparsed_query[i] == '?') {
				if(in_path) {
					request->path[i] = 0;
					in_path = 0;
				}
				in_query_string = 1;
			}
			else {
				if(in_path)
					request->path[i] = unparsed_query[i];
				if(in_query_string)
					request->query[q_counter++] = unparsed_query[i];
			}
		}
	}
	else
		return 0;

	printf("p:%s q:%s\n", request->path, request->query);
	return 1;
}

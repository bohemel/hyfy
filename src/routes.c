#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "hyfy.h"
#include "routes.h"

struct route routes[] = { 
	{"/", 		RT_STATIC, "resources/html/index.html", NULL, NULL}, 
	{"/index.html", RT_STATIC, "resources/html/index.html", NULL, NULL}, 
	{"/hyfy.js", 	RT_STATIC, "resources/js/hyfy.js", NULL, NULL}
};

const int NO_OF_ROUTES = 3;

void routes_init() {

	for(int i = 0; i < NO_OF_ROUTES; i++) {
		hyfy_log("Loading files into memory..");
		if(routes[i].type == RT_STATIC) {
			hyfy_log(routes[i].static_content_source);
			if(routes_load_static_file(routes[i].static_content_source, &routes[i].static_content) < 0)
				hyfy_error("Failed loading file, exiting");
		}
	}	
}

int routes_load_static_file(const char *filename, char **result) { 
	int size = 0;
	FILE *f = fopen(filename, "rb");
	if (f == NULL) { 
		*result = NULL;
		return -1; // -1 means file opening fail 
	} 
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	*result = (char *)malloc(size+1);
	if (size != fread(*result, sizeof(char), size, f)) 
	{ 
		free(*result);
		return -2; // -2 means file reading fail 
	} 
	fclose(f);
	(*result)[size] = 0;
	return size;
}

int routes_match(struct http_request* request, struct route **active_route) {
	hyfy_log("In routes match...");
	for(int i = 0; i < NO_OF_ROUTES; i++) {
		printf("Comparing %s with %s\n", request->request_path, routes[i].request_string );
		if(strcmp(request->request_path, routes[i].request_string) == 0) {
			*active_route = &routes[i];
			return 1;
		}
	}
	return 0;
}



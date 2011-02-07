#include <stdio.h>

#include "response.h"
#include "api.h"

void api_test(struct http_request* request) {
	http_response_send("200 OK", "text/plain", request, "Echo");
}

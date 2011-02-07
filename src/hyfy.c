#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "hyfy.h"
#include "routes.h"
#include "request.h"
#include "response.h"

#define BUFSIZE 8096

void error(const char *msg) {
	perror(msg);
	exit(1);
}

void hyfy_log(const char *msg) {
	printf("%s\n", msg);
}

void hyfy_error(const char *msg) {
	perror(msg);
	exit(1);
}

void dispatch_request(int socket_fd) {

	static char buffer[BUFSIZE + 1];
	static struct http_request request;

	request.socket_fd = socket_fd;

	long ret = read(socket_fd,buffer,BUFSIZE); 	// read Web request in one go
	if(ret > 0 && ret < BUFSIZE)			// return code is valid chars 
		buffer[ret]=0;				// terminate the buffer
	else buffer[0]=0;

	if(parse_http_request(buffer, &request) == 0) {
		hyfy_error("Could not parse incomming request");
	}

	struct route *active_route;
	if(routes_match(&request, &active_route) == 0) {
	//	http_response_404(request);
		sprintf(buffer,"HTTP/1.0 404 Not Found\r\nServer: Born to be wild 0.1\r\nContent-Type: text/plain\r\n\r\nResource not found... Sorry dude.. real sorry...");
		write(socket_fd,buffer,strlen(buffer));
	}
	else {
		if(active_route->type == RT_STATIC) {
			hyfy_log("Found static route:");
			hyfy_log(active_route->static_content_source);
			http_response_send("200 OK", active_route->content_type, &request, active_route->static_content);
		}
		else {
			printf("Could not handle matching route!");
		}
	}
	destroy_request(&request);
} 

void destroy_request(struct http_request *request) {
	close(request->socket_fd);
} 

int main() {

	printf("hyfy now online on port 8080 \n");

	// load routes 
	routes_init();

	int sockfd, newsockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		hyfy_error("ERROR opening socket");

	int portno = 8080;
	struct sockaddr_in serv_addr, cli_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		hyfy_error("ERROR on binding");

	listen(sockfd,5);

	socklen_t clilen;
	clilen = sizeof(cli_addr);

	while(1) {
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
		if (newsockfd < 0) 
			hyfy_error("ERROR on accept");
		
		// handle new connection
		dispatch_request(newsockfd);
	}

	close(sockfd);
	return 0;
}
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 8096

void error(const char *msg) {
	perror(msg);
	exit(1);
}

void hyfy_log(const char *msg) {
	printf("%s\n", msg);
}

void requesthandler(int socket_fd) {
	static char buffer[BUFSIZE+1]; /* static so zero filled */
	long ret;

	hyfy_log("Got Request!");
	
	ret = read(socket_fd,buffer,BUFSIZE); 	/* read Web request in one go */
	if(ret > 0 && ret < BUFSIZE)	/* return code is valid chars */
		buffer[ret]=0;		/* terminate the buffer */
	else buffer[0]=0;

	printf("%s", buffer);
	

	sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\nIt's the shit yao!");
	write(socket_fd,buffer,strlen(buffer));
	close(socket_fd);
} 

int main() {

	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	printf("HyFy!\n");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	portno = 8080;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	listen(sockfd,5);
	clilen = sizeof(cli_addr);


	while(1) {
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");
		requesthandler(newsockfd);
	}

	close(sockfd);
	return 0;
}
 

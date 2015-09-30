#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
void err_sys(char* message);
int main(int argc, char* argv[]){
	int sockfd, newsockfd;
	int portno;
	int clilength;
	int n;
	char buff[256];
	struct sockaddr_in serv_addr, cli_addr;
	if(argc < 2)
		err_sys("usage ./webserver <port number>");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		err_sys("failed to create socket");
	}
	bzero((char*) &serv_addr, sizeof(serv_addr));
	bzero(buff, 256);
	portno = atoi(argv[1]);//not needed
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
		err_sys("error on binding\n");
	if(listen(sockfd, 5) < 0)
		err_sys("error on listen");
	clilength = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilength);
	if(newsockfd < 0)
		err_sys("error on accept\n");
	n = read(newsockfd, buff, 255);
	if(n < 0)
		err_sys("error on read");
	printf("%s\n", buff);
	if(strcmp(buff, "GET")){
		char* header = "HTTP/1.1 200 ok/nServer: MyWeb/nContact-Type: text/html;\n";
		int l = strlen(header);
		n = write(newsockfd, header, l);
		if(n < 0)
			err_sys("error on write");
	}
	return 0;
}
void err_sys(char* message){
	printf("%s\n", message);
	exit(0);
}

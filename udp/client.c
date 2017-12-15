#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "wrap.h"


#define PORT 8000
#define MAXLINE 1024
int main(int argc, char* agrv[])
{
	char buf[MAXLINE];
	memset(buf, 0, sizeof(buf));
	int server_id = Socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in server;
	bzero(&server, sizeof(server));	
	server.sin_family = PF_INET;
	server.sin_port = htons(PORT);
	inet_pton(PF_INET, "127.0.0.1", &server.sin_addr);


	while (fgets(buf, MAXLINE, stdin) != NULL) {
		sendto(server_id, buf, strlen(buf), 0, (struct sockaddr*)&server, sizeof(server));
		int n = recvfrom(server_id, buf, sizeof(buf), 0, NULL, 0);
		Write(STDOUT_FILENO, buf, n);
	}
	Close(server_id);
	return 0;
}

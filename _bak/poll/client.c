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
	int server_id = Socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server;
	bzero(&server, sizeof(server));	
	server.sin_family = PF_INET;
	server.sin_port = htons(PORT);
	inet_pton(PF_INET, "127.0.0.1", &server.sin_addr);

	Connect(server_id, (struct sockaddr*)&server, sizeof(server));

	while (fgets(buf, MAXLINE, stdin) != NULL) {
		Write(server_id, buf, strlen(buf));
		int n = Read(server_id, buf, MAXLINE);
		if (n == 0) {
			printf("the other side has been closed.\n");
		} else {
			Write(STDOUT_FILENO, buf, n);
		}
	}
	Close(server_id);
	return 0;
}

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


#define PORT 8000
#define MAXLINE 1024
int main(int argc, char* agrv[])
{
	if (argc != 2)
	{
		fputs("usage: ./client message\n", stderr);
		exit(1);
	}

	char* str = agrv[1];
	char buf[MAXLINE];
	memset(buf, 0, sizeof(buf));
	int server_id = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server;
	bzero(&server, sizeof(server));	
	server.sin_family = PF_INET;
	server.sin_port = htons(PORT);
	inet_pton(PF_INET, "127.0.0.1", &server.sin_addr);

	connect(server_id, (struct sockaddr*)&server, sizeof(server));
	while(1){}
	write(server_id, str, strlen(str));

	int n = read(server_id, buf, MAXLINE);
	printf("Response from server:\n");
	write(STDOUT_FILENO, buf, n);

	close(server_id);
	return 0;
}

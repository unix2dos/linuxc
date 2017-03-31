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
int main()
{
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int server_id = Socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server, client;
	bzero(&server, sizeof(server));	
	server.sin_family = PF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(server_id, (struct sockaddr*)&server, sizeof(server));
	Listen(server_id, 20);
	printf("Accept connections...\n");


	while (1)
	{
		socklen_t len = sizeof(client);
		int client_id = Accept(server_id, (struct sockaddr*)&client, &len); 

		while (1) {
			int n = Read(client_id, buf, sizeof(buf));
			if (n == 0) {
				printf("the other side has been closed.\n");
				break;	
			}
			printf("received from %s at PORT %d\n",
					inet_ntop(PF_INET, &client.sin_addr, str, sizeof(str)),	
					ntohs(client.sin_port));
			for (int i = 0; i < n; ++i) {
				buf[i] = toupper(buf[i]);
			}
			Write(client_id, buf, n);
		}

		Close(client_id);
	}

	return 0;
}

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
	int server_id = Socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in server, client;
	bzero(&server, sizeof(server));	
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(server_id, (struct sockaddr*)&server, sizeof(server));
	printf("Accept connections...\n");


	while (1)
	{
		socklen_t len = sizeof(client);
		int n = recvfrom(server_id, buf, sizeof(buf), 0, (struct sockaddr*)&client, &len);
		printf("received from %s at PORT %d\n",
				inet_ntop(AF_INET, &client.sin_addr, str, sizeof(str)),	
				ntohs(client.sin_port));
		for (int i = 0; i < n; ++i) {
			buf[i] = toupper(buf[i]);
		}
		sendto(server_id, buf, n, 0, (struct sockaddr *)&client, sizeof(client));
	}

	Close(server_id);

	return 0;
}

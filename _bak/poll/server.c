#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/select.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include "wrap.h"

#define PORT 8000
#define MAXLINE 1024
#define OPEN_MAX 1000
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
	//不同地址相同端口可以监听
	int opt = 1;
	setsockopt(server_id, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	Bind(server_id, (struct sockaddr*)&server, sizeof(server));
	Listen(server_id, 20);
	printf("Accept connections...\n");


	struct pollfd clients[OPEN_MAX];
	clients[0].fd = server_id;
	clients[0].events = POLLIN;
	for (int i = 1; i < OPEN_MAX; i++) {
		clients[i].fd = -1;
	}

	int maxi = 0;
	while (1) {
		int iready = poll(clients, maxi+1, -1);	
		if (iready < 0) {
			perr_exit("poll error");
		}

		if (clients[0].revents & POLLIN) {
			socklen_t len = sizeof(client);
			int client_id = Accept(server_id, (struct sockaddr*)&client, &len);	
			printf("received from %s at PORT %d\n",
					inet_ntop(PF_INET, &client.sin_addr, str, sizeof(str)),
					ntohs(client.sin_port));

			int i = 1;
			for (; i < OPEN_MAX; ++i) {
				if (clients[i].fd < 0) {
					clients[i].fd = client_id;	
					break;
				}
			}

			if (i == OPEN_MAX) {
				fputs("too many clients\n", stderr);
				exit(1);
			}

			clients[i].events = POLLIN;
			if (i > maxi) {
				maxi = i;
			}
			if (--iready == 0) {
				continue;
			}
		}

		for (int i = 1; i <= maxi; ++i) {
			if (clients[i].fd < 0) {
				continue;
			}	

			if (clients[i].revents & POLLIN) {
				int n = Read(clients[i].fd, buf, sizeof(buf));
				if (n == 0) {
					Close(clients[i].fd);
					clients[i].fd = -1;
				} else {
					for (int i = 0; i < n; ++i) {
						buf[i] = toupper(buf[i]);
					}
					Write(clients[i].fd, buf, n);
				}
				if (--iready == 0) {
					break;
				}
			}
		}
	}
	return 0;
}

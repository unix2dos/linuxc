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
	//不同地址相同端口可以监听
	int opt = 1;
	setsockopt(server_id, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	Bind(server_id, (struct sockaddr*)&server, sizeof(server));
	Listen(server_id, 20);
	printf("Accept connections...\n");

	int clients[FD_SETSIZE]; 
	for (int i = 0; i < FD_SETSIZE; ++i) {
		clients[i] = -1;
	}
	fd_set rset, allset;
	FD_ZERO(&allset);
	FD_SET(server_id, &allset);
	int maxfd = server_id;
	int maxi = -1;

	while (1) {
		rset = allset;
		int iready = select(maxfd+1, &rset, NULL, NULL, NULL);
		if (iready < 0) {
			perr_exit("select error");
		}

		if (FD_ISSET(server_id, &rset)) {
			//new client
			socklen_t len = sizeof(client);
			int client_id = Accept(server_id, (struct sockaddr*)&client, &len);
			printf("received from %s at PORT %d\n",
					inet_ntop(PF_INET, &client.sin_addr, str, sizeof(str)),	
					ntohs(client.sin_port));

			int i = 0;
			for (; i < FD_SETSIZE; ++i) {
				if (clients[i] < 0) {
					clients[i] = client_id;
					break;
				}
			}
			if (i == FD_SETSIZE) {
				fputs("too many clients\n", stderr);
				exit(1);
			}
			FD_SET(client_id, &allset);
			if (client_id > maxfd) {
				maxfd = client_id;
			}
			if (i > maxi) {
				maxi = i;
			}
			if (--iready == 0) {
				continue;
			}
		}

		for (int i = 0; i <= maxi; ++i) {
			int fd = clients[i];
			if (fd < 0) {
				continue;	
			}
			if (FD_ISSET(fd, &rset)) {
				int n = Read(fd, buf, sizeof(buf));
				if (n == 0) {
					Close(fd);
					FD_CLR(fd, &allset);
					clients[i] = -1;
				} else {
					for (int i = 0; i < n; ++i) {
						buf[i] = toupper(buf[i]);
					}
					Write(fd, buf, n);
				}
				if (--iready == 0) {
					break;
				}
			}
		}
	}

	return 0;
}

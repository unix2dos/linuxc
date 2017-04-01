#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "wrap.h"
#include "epollUtil.h"

#define IP "127.0.0.1"
#define PORT 8000
#define FDSIZE 1024
#define EPOLLEVENTS 100


int main(int argc, char* argv[]) {
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	char str[128];
	struct sockaddr_in server, client;
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	inet_pton(AF_INET, IP, &server.sin_addr);

	int server_id = Socket(AF_INET, SOCK_STREAM, 0);
	Bind(server_id, (struct sockaddr*)&server, sizeof(server));
	Listen(server_id, 20);
	printf("Accept connection...\n");	

	struct epoll_event events[EPOLLEVENTS];
	int epollfd = epoll_create(FDSIZE);
	add_event(epollfd, server_id, EPOLLIN); //监听读

	while (1) {
		int ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);	

		for (int i = 0; i < ret; ++i) {
			int fd = events[i].data.fd;

			if (fd == server_id && (events[i].events & EPOLLIN)) {
				socklen_t len = sizeof(client);
				int client_id = Accept(server_id, (struct sockaddr*)&client, &len);
				printf("accept new client %s:%d\n", inet_ntop(AF_INET, &client.sin_addr, str, sizeof(str)), ntohs(client.sin_port));
				add_event(epollfd, client_id, EPOLLIN);	
			} else if (events[i].events & EPOLLIN) {
				int n = Read(fd, buf, sizeof(buf));	
				if (n == 0) {
					Close(fd);
					delete_event(epollfd, fd, EPOLLIN);
				} else {
					modify_event(epollfd, fd, EPOLLOUT);	
				}
			} else if (events[i].events & EPOLLOUT) {
				for (int i = 0; i < strlen(buf); ++i) {
					buf[i] = toupper(buf[i]);
				}
				 Write(fd, buf, strlen(buf));	
				 modify_event(epollfd, fd, EPOLLIN);	
			}
		}
	}

	Close(epollfd);
	return 0;
}

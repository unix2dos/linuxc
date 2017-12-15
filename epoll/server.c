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
#include <sys/epoll.h>
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


	struct epoll_event events[EPOLLEVENTS];
	int epollfd = epoll_create(FDSIZE);

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = STDIN_FILENO;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);

	while (1) {
		int ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
		for (int i = 0; i < ret; ++i) {
			int fd = events[i].data.fd;
			if (fd == server_id && (events[i].events & EPOLLIN)) {
				socklen_t len = sizeof(client);
				int client_id = Accept(server_id, (struct sockaddr*)&client, &len);	
				printf("received from %s at PORT %d\n",
						inet_ntop(PF_INET, &client.sin_addr, str, sizeof(str)),
						ntohs(client.sin_port));

				struct epoll_event ev;
				ev.events = state;
				ev.data.fd = fd;
				epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);

			} else if (events[i].events & EPOLLIN) {
				int n = Read(clients[i].fd, buf, sizeof(buf));
				if (n == 0) {
					Close(clients[i].fd);

					struct epoll_event ev;
					ev.events = EPOLLIN;
					ev.data.fd = fd;
					epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);

				} else {
					struct epoll_event ev;
					ev.events = EPOLLOUT;//由读改为写
					ev.data.fd = fd;
					epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
				}

			} else if (events[i].events & EPOLLOUT) {
				for (int i = 0; i < n; ++i) {
					buf[i] = toupper(buf[i]);
				}
				int n = Write(fd, buf, n);
				if (n < 0) {
					struct epoll_event ev;
					ev.events = EPOLLIN;
					ev.data.fd = fd;
					epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);

				} else {
					struct epoll_event ev;
					ev.events = EPOLLIN;//由写改为读
					ev.data.fd = fd;
					epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
				}
			}
		}
	}

	Close(epollfd);


	return 0;
}

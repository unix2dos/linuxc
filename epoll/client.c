#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "wrap.h"
#include "epollUtil.h"

#define IP "127.0.0.1"
#define PORT 8000
#define FD_SIZE 1024
#define EPOLLEVENTS 20
int main(int agrc, char* argv[]) {
	char buf[1024];
	memset(buf, 0, sizeof(buf));

	struct sockaddr_in server;
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	inet_pton(AF_INET, IP, &server.sin_addr);
	

	int server_id = Socket(AF_INET, SOCK_STREAM, 0);
	Connect(server_id, (struct sockaddr*)&server, sizeof(server));

	struct epoll_event events[EPOLLEVENTS];
	int epollfd = epoll_create(FD_SIZE);
	add_event(epollfd, STDIN_FILENO, EPOLLIN);
	while (1) {
	
		int ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
		for (int i = 0; i < ret; ++i) {
			int fd = events[i].data.fd;

			if (events[i].events & EPOLLIN) {
				int n = Read(fd, buf, sizeof(buf));	
				if (n == 0) {
					Close(fd);	
				} else {
					if (fd == STDIN_FILENO) {
						add_event(epollfd, server_id, EPOLLOUT);
					} else {
						delete_event(epollfd, server_id, EPOLLIN);	
						add_event(epollfd, STDOUT_FILENO, EPOLLOUT);
					}	
				}
			} else if (events[i].events & EPOLLOUT) {
				Write(fd, buf, strlen(buf));	
				if (fd == STDOUT_FILENO) {
					delete_event(epollfd, fd, EPOLLOUT);
				} else {
					modify_event(epollfd, fd, EPOLLIN);
				}
			}
		}
	}

	Close(server_id);
	return 0;
}

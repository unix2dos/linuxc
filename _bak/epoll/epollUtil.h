#ifndef _EPOLL_H_
#define _EPOLL_H_

void add_event(int epollid, int fd, int state)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = state;
	epoll_ctl(epollid, EPOLL_CTL_ADD, fd, &ev);
}
void modify_event(int epollid, int fd, int state)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = state;
	epoll_ctl(epollid, EPOLL_CTL_MOD, fd, &ev);
}
void delete_event(int epollid, int fd, int state)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = state;
	epoll_ctl(epollid, EPOLL_CTL_DEL, fd, &ev);
}


#endif

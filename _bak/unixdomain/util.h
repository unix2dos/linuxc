#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include "wrap.h"



#define offsetof(TYPE, MEMBER) ((int)&((TYPE*)0)->MEMBER)
#define CLI_PATH "/var/tmp"
#define NAME "foo.socket"

int serv_listen(const char* name) {
	unlink(name);

	int fd = Socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un un;
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	printf("serv_listen sun_path = %s\n", un.sun_path);

	int len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
	Bind(fd, (struct sockaddr*)&un, len);
	Listen(fd, 10);
	return fd;
}


int serv_accept(int listenfd, uid_t *uidptr) {

	struct sockaddr_un un;

	socklen_t len = sizeof(un);
	int client_id = Accept(listenfd, (struct sockaddr*)&un, &len);
	len -= offsetof(struct sockaddr_un, sun_path);//path name length
	un.sun_path[len] = 0;//add \0

	struct stat statbuf;
	stat(un.sun_path, &statbuf);
	if (S_ISSOCK(statbuf.st_mode) == 0) {
		Close(client_id);
		return -1;
	}

	if (uidptr != NULL) {
		*uidptr = statbuf.st_uid;
	}

	printf("serv_accept sun_path = %s uid = %d \n", un.sun_path, *uidptr);

	unlink(un.sun_path);//del path
	return client_id;
}


int cli_conn(const char* name) {

	int fd = Socket(AF_UNIX, SOCK_STREAM, 0);

	struct sockaddr_un un;
	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	sprintf(un.sun_path, "%s%05d", CLI_PATH, getpid());
	printf("cli sun_path = %s\n", un.sun_path);
	socklen_t len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	unlink(un.sun_path);
	Bind(fd, (struct sockaddr*)&un, len);
	


	memset(&un, 0, sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, name);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	Connect(fd, (struct sockaddr*)&un, len);
	return fd;	
}


#endif 

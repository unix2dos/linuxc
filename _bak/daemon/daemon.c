#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


void daemonize() {
	pid_t id = fork();
	if (id > 0) {
		exit(0);
	}

	setsid();//create new session  (deamon)

	if (chdir("/") < 0) {
		perror("chdir");
		exit(1);
	} 

	close(0);
	open("/dev/null", O_RDWR);
	dup2(0, 1);
	dup2(0, 2);
}


int main(int argc, char* argv[]) {

	daemonize();
	while (1);
	return 0;
}

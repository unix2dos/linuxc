#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {

	int fileds[2];
	pipe(fileds);
	
	pid_t id = fork();
	if (id == 0) {
		close(fileds[1]);

		char buf[1024];
		read(fileds[0], buf, sizeof(buf));
		printf("%s", buf);
	
	}else if (id > 0) {
		close(fileds[0]);
		const char* text = "levonfly is a boy\n";
		write(fileds[1], text, strlen(text));	
		wait(NULL);
	}


	return 0;
}

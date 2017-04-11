#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>



int main (int argc, char* argv[]) {
	if (argc < 2) {
		printf("agrc < 2\n");
		return -1;
	} 

	char* name = argv[1];//file name
	int fd = open(name, O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);

	execl("./upper", "upper", NULL);//execl之后文件描述符还是打开

	return 0;
}

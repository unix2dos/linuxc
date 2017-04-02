#include "util.h"


int main(int argc, char* argv[]) {
	char buf[1024];
	int server_id = cli_conn(NAME);

	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		Write(server_id, buf, strlen(buf));	
		int n = Read(server_id, buf, sizeof(buf));
		if (n == 0) {
			printf("the other side has been closed.\n");
		} else {
			Write(STDOUT_FILENO, buf, n);
		}
	}
	return 0;
}

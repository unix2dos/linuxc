#include "util.h"

int main(int argc, char* argv[]) {

	printf("UNIX domain socket bound\n");
	uid_t uid;
	char buf[1024];
	int serve_id = serv_listen(NAME);
	int client_id = serv_accept(serve_id, &uid); 

	while (1) {
		int n = Read(client_id, buf, sizeof(buf));
		for (int i = 0; i < n; ++i) {
			buf[i] = toupper(buf[i]);
		}
		Write(client_id, buf, n);
	}
	return 0;
}

#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int main(){

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1){
		printf("Error creating socket!\n");
		return -1;
	}

	struct sockaddr_in serverinfo;
	serverinfo.sin_family = AF_INET;
	serverinfo.sin_port = htons(1234);
	serverinfo.sin_addr.s_addr = inet_addr("172.30.252.120");


	int connect_res = connect(socket_fd, (const struct sockaddr*)&serverinfo,
			sizeof(serverinfo));
	if(connect_res == -1){
		printf("Could not establish connection to server...\n");
	}
	while(1){
		printf("Connected to server...\n");
		sleep(1000);
	}

	return 0;
}


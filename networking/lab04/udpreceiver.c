#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>


int main(){
	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_fd == -1){
		printf("Socket error\n");
		return -1;
	}
	int broadcast = 1;
	int resultsetsockopt = setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST,
			&broadcast, sizeof(broadcast));
	if (resultsetsockopt == -1){
		printf("Setsockopt error\n");
		return -2;
	}

	

	struct sockaddr_in recv_addr;
	recv_addr.sin_family = AF_INET;
	recv_addr.sin_port = htons(1234);
	recv_addr.sin_addr.s_addr = INADDR_ANY;

	int res_bind = bind(socket_fd, (const struct sockaddr *) &recv_addr, 
			sizeof(recv_addr));
	if (res_bind == -1){
		printf("Binding error\n");
		return -3;
	}

	struct sockaddr_in sender_addr;
	char message[100];
	socklen_t sender_addr_len;

	while (1){
		int result_recvfrom = recvfrom(socket_fd, &message, 100, 0,
				(struct sockaddr *)&sender_addr, &sender_addr_len);
		if(result_recvfrom == -1){
			printf("Couldn't receive message\n");
			continue;
		}
		printf("%s\n", message);
	}
	return 0;
}

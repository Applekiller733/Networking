#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main(){
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1){
		printf("Socket error\n");
		return -1;
	}

	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(1234);
	client.sin_addr.s_addr = inet_addr("192.168.1.142");

	int connect_res = connect(socket_fd, (const struct sockaddr*)&client,
						sizeof(client));

	if (connect_res == -1){
		printf("Couldn't connect to server\n");
		return -2;
	}
	printf("Connected to server...\n");
	char buffer[100];
	int buff_size = recv(socket_fd, buffer, sizeof(buffer), 0);
	if (buff_size == -1){
		printf("Could not receive message from server\n");
		return -3;
	}
	printf("Received message: %s \n", buffer);
	char response[100] = "Zelutzu\0";
	int send_res = send(socket_fd, response, strlen(response)+1, 0);
	if (send_res == -1){
		printf("Could not send message to server\n");
		return -4;
	}
	return 0;
}

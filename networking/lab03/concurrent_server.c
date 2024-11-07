#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1 ){
		printf("Failed to create socket!\n");
		return -1;
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(1234);
	server.sin_addr.s_addr = INADDR_ANY;
	
	int bind_res = bind(socket_fd, (const struct sockaddr*) &server, 
							sizeof(server));
	if (bind_res == -1){
		printf("Bind error\n");
		return -2;
	}
	
	int listen_res = listen(socket_fd, 7);
	if (listen_res == -1){
		printf("Listen error\n");
		return -3;
	}

	struct sockaddr_in client;
	socklen_t client_len;
	while (1) {
		printf("Expecting clients...\n");
		int client_fd = accept(socket_fd,
				(struct sockaddr*)&client, &client_len);
		if (client_fd == -1) {
			printf("Client couldn't connect \n");
			continue;
		}
		if (fork() == 0){
		printf("Client with IP addr: %s and port %d has connected...\n",
				inet_ntoa(client.sin_addr), ntohs(client.sin_port));		   
		char buffer[100] = "Do you want VIP status?\n If yes, type in your username\n";
		int sent_len = send(client_fd, buffer, strlen(buffer)+1, 0);
		if (sent_len == -1) {
			printf ("Couldn't send message to client\n");
			exit(-1);
		}
		char readbuffer[100];
		int read_len = recv(socket_fd, readbuffer, 100, 0);
		if (read_len == -1){
			printf("Could not read message from client\n");
			exit(-2);
		}
		if (strcmp(readbuffer, "no") != 0){
			printf("User chose VIP name: %s\n", readbuffer);
		}
		else {
			printf("User doesn't want VIP\n");
		}
		close(client_fd);
		exit(0);
		}
	}
	return 0;
}

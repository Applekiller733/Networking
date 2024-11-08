#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

void* udp_receiver(void* arg){
	//printf("UDP receiver thread started\n");
	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_fd == -1){
		perror("socket");
		return NULL;
	}

	int broadcast = 1;
	int setsockopt_res = setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	if (setsockopt_res == -1){
		perror("setsockopt");
		return NULL;
	}

	struct sockaddr_in broadcastinfo;
	memset(&broadcastinfo, 0, sizeof(broadcastinfo));
	broadcastinfo.sin_family = AF_INET;
	broadcastinfo.sin_port = htons(1235);
	broadcastinfo.sin_addr.s_addr = INADDR_ANY;

	int bind_res = bind(socket_fd, (const struct sockaddr*)&broadcastinfo, sizeof(broadcastinfo));
	if(bind_res == -1){
		printf("bind\n");
		return NULL;
	}

	struct sockaddr_in senderinfo;
	char buffer[100];
	socklen_t sender_len;

//	printf("Reached while loop\n");
	while(1){
//		printf("Waiting for message\n");
		int recvfrom_res = recvfrom(socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&senderinfo, &sender_len);
		if(recvfrom_res == -1){
			printf("Error at recvfrom\n");
			continue;
		}
		
		printf("Received: %s from %s:%d\n", buffer, inet_ntoa(senderinfo.sin_addr), ntohs(senderinfo.sin_port));
		
	}
}

int main(){
	pthread_t udp_receiver_thread;
	int create_res = pthread_create(&udp_receiver_thread, NULL, udp_receiver, NULL);
	if (create_res != 0){
		perror("pthread_create");
		return 1;
	}


	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd == -1){
		perror("socket");
		return 1;
	}

	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(1234);
	server.sin_addr.s_addr = inet_addr("192.168.1.136");

	int connect_res = connect(socket_fd, (struct sockaddr*)&server, sizeof(server));
	if(connect_res == -1){
		perror("Error at connect\n");
		return 1;
	}

	printf("Connected to server\n");
	while(1){
		
	}
	return 0;
}

#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <unordered_map>
#include <string>
#include <pthread.h>

void* udp_broadcast_routine(void* args){
	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_fd == -1){
		printf("Error creating socket\n");
		return NULL;
	}

	int yes = 1;
	int setsockopt_res = setsockopt(socket_fd, SOL_SOCKET,
			SO_BROADCAST, &yes, sizeof(yes));
	if (setsockopt_res == -1){
		printf("Error setsockopt\n");
		return NULL;
	}

	struct sockaddr_in sender;
	memset(&sender, 0, sizeof(struct sockaddr_in));
	sender.sin_family = AF_INET;
	sender.sin_port = htons(1235);
	sender.sin_addr.s_addr = inet_addr("192.168.1.255");

	while (1) {
		char buffer[] = "Alexandra, 172.30.248.40, 808\n H.F. Pop, 172.30.246.143, 8082\n Gabitzu, 172.30.245.22, 8083\n Applekiller, 192.168.1.136, 8084\n";	
		int send_res = sendto(socket_fd, buffer, strlen(buffer)+1, 0,
				(struct sockaddr*)&sender, sizeof(sender));
		if (send_res == -1){
			printf("Error sending\n");
			continue;
		}
		sleep(1);
	}
	return NULL;
}

int main(){
	pthread_t udp_broadcast_thread;
	pthread_create(&udp_broadcast_thread, NULL, udp_broadcast_routine, NULL);

	std::unordered_map<in_addr_t, std::string> user_data;

	user_data[inet_addr("172.30.246.143")] = "H.F. Pop";
	user_data[inet_addr("172.30.248.40")] = "Alexandra";
	user_data[inet_addr("172.30.245.22")] = "Gabitzu";
	user_data[inet_addr("192.168.1.136")] = "Applekiller";

	fd_set master;
	fd_set read_fds;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1){
		printf("Error creating socket\n");
		return -1;
	}
	
	int yes = 1;
	int setsockopt_res = setsockopt(socket_fd, SOL_SOCKET,
			SO_REUSEADDR, &yes, sizeof(yes));
	if (setsockopt_res == -1){
		printf("Error setsockopt\n");
		return -2;
	}

	struct sockaddr_in serverinfo;
	memset(&serverinfo, 0, sizeof(struct sockaddr_in));
	serverinfo.sin_family = AF_INET;
	serverinfo.sin_port = htons(1234);
	serverinfo.sin_addr.s_addr = INADDR_ANY;

	int bind_res = bind(socket_fd, (const struct sockaddr*)&serverinfo,
			sizeof(serverinfo));
	if (bind_res == -1){
		printf("Error binding\n");
		return -3;
	}

	int listen_res = listen(socket_fd, 7);
	if(listen_res == -1){
		printf("Error at listen\n");
		return -4;
	}

	FD_SET(socket_fd, &master);
	int fdmax = socket_fd;
	
	for(;;){
		printf("Waiting for clients...\n");
		read_fds = master;
		int select_res = select(fdmax+1, &read_fds, NULL, NULL, NULL);
		if (select_res == -1){
			printf("Error at select\n");
			return -5;
		}
		
		for (int i = 0; i <= fdmax; i ++){
			if (FD_ISSET(i, &read_fds)){
				
				if (i == socket_fd){
					struct sockaddr_in clientinfo;
					memset(&clientinfo, 0, 
							sizeof(clientinfo));
					socklen_t clientlen;
					int client_fd = accept(socket_fd,
						(struct sockaddr*)&clientinfo, &clientlen);
					if (client_fd == -1){
						printf("Client could not connect...\n");
						continue;
					}
					FD_SET(client_fd, &master);
					if (client_fd > fdmax){
						fdmax = client_fd;
					}
					
					if (user_data.find(clientinfo.sin_addr.s_addr)
							!= user_data.end()){
						printf("%s connected\n",
							user_data[clientinfo.sin_addr.s_addr].c_str());
					}
					else{
						printf("Client connected with IP: %s\n",
						inet_ntoa(clientinfo.sin_addr));
					}
					close(client_fd);
					FD_CLR(client_fd, &master);
				}
			}
		}
	}
	return 0;
}


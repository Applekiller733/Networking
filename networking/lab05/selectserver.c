#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(){
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
				int crt = i;
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
					
					printf("Client connected with IP:%s:%d\n",
							inet_ntoa(clientinfo.sin_addr),
							ntohs(clientinfo.sin_port));
					close(client_fd);
					FD_CLR(client_fd, &master);
				}
			}
		}
	}
	return 0;
}


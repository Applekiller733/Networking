#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_CONNECTED_LIST_SIZE 100;

struct connected_client_list{
	struct sockaddr_in client;
	int client_fd;
	
}list[MAX_CONNECTED_LIST_SIZE];
int connected_client_list_index=0;

void add_client_to_list(struct sockaddr_in client, int client_fd){
	if (connected_client_list_index < MAX_CONNECTED_LIST_SIZE){
		list[connected_client_list_index].client = client;
		list[connected_client_list_index].client_fd = client_fd;
		connected_client_list_index += 1;
	}
	else
		printf("Connected client list is full!\n");
}

void remove_client_from_list(int index){
	for (int i = index, i < connected_client_list_index-1; i++){
		list[i] = list[i+1];
	}
	connected_list_index -= 1;
}


int main(){
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1){
		printf("Error creating socket!\n");
		return -1;
	}

	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(1234);
	client.sin_addr.s_addr = INADDR_ANY;
	socklen_t clientlen;

	int bind_res = bind(socket_fd, (const struct sockaddr*)&client, sizeof(client));
	if (bind_res == -1){
		printf("Bind error\n");
		return -2;
	}

	int listen_res = listen(socket_fd, 5);
	if (listen_res == -1){
		printf("Listen error\n");
		return -3;
	}

	while(1){
		int client_fd = accept(socket_fd, (struct sockaddr*)&client,
				&clientlen);
		if(client_fd == -1){
			printf("Client could not connect...\n");
			continue;
		}
		add_client_to_list(client, client_fd);
		printf("Added client with IP:%s to connected clients list...\n",
				inet_ntoa(client.sin_addr));
	}
	return 0;
}

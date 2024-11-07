#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int main(){
	srand(time(NULL));	
	struct sockaddr_in serverinfo;
	serverinfo.sin_family = AF_INET;
	serverinfo.sin_port = htons(1234);
	serverinfo.sin_addr.s_addr = inet_addr("192.168.1.142");

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if ( socket_fd == -1 ){
		printf("Error creating socket!\n");
		return -1;
	}

/*	int bind_res = bind(socket_fd, (const struct sockaddr *)&serverinfo,
			sizeof(serverinfo));

	if (bind_res == -1){
		printf("Error binding socket!\n");
		return -2;
	}
*/
	int connect_res = connect(socket_fd, (const struct sockaddr *)&serverinfo,
			sizeof(serverinfo));

	if (connect_res == -1){
		printf("Could not connect to server\n");
		return -3;
	}
	
	float random_val = (float) rand() / (float)(RAND_MAX / 50);
	printf("Connected to server...\n");
	printf("Sending value %f to server...\n", random_val);
	int send_res = send(socket_fd, &random_val, sizeof(float), 0);
	if (send_res == -1){
		printf("Could not send information to server\n");
		return -4;
	}
	
		char servermessage[100];
		int mssg_size = recv(socket_fd, servermessage, 
				sizeof(servermessage), 0);
		if (mssg_size == -1){
			printf("Could not receive message from server!\n
					Terminating connection...\n");
			return -5;
		}
		printf("%s\n", servermessage);
	
	return 0;
}

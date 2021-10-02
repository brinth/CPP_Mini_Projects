#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
	printf("Creating a Socket\n");
	int client_sd = socket(AF_INET, SOCK_STREAM, 0);	
	if(client_sd <= 0)
	{
		printf("Error Creating Socket\n");
		return 1;
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(7878);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	connect(client_sd, (struct sockaddr*) &server_addr, sizeof(server_addr));
	char payload[500] = {};
	recv(client_sd, &payload, sizeof(payload), 0);
	// Printing received payload
	printf("Received %s\n", payload);
	close(client_sd);
	return 0;
}

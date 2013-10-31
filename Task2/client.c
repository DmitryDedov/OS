#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main()
{
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	char inputString[1000];

	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "server_socket");
	len = sizeof(address);

	result = connect(sockfd, (struct sockaddr *)&address, len);
	if (result == -1)
	{
		perror("opps: client");
		exit(1);
	}
	while(1)
	{
		printf("Input text: ");
		cin.getline(inputString, 1000);

		write(sockfd, inputString, 1000);
		read(sockfd, inputString, 1000);
		printf("%s\n", inputString);
	}
	close(sockfd);
	exit(0);
} 
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>		

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;

	unlink("server_socket");
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);
	client_len = sizeof(client_address);
	client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
	while(1)
	{
		char inputString[1000]; //пришедшая строка от клиента 
		char path[1000] = "/bin/";

		read(client_sockfd, inputString, 1000); //принимает от клиента строку

		char *commandString;
		commandString = strtok(inputString, " "); // разбиваем строку на слова

		while(commandString != NULL)
		{
			printf("%s\n", commandString);
			commandString = strtok (NULL, " ");
		}

		strcat(path, inputString);
		printf("%s\n", path);
		if (access(path, 0) == -1)
		{
	 		write(client_sockfd, "Command not found!", 1000);
		}
		else
		{
			if (fork() == 0)
			{
			    dup2(client_sockfd, 1);
				execl(path, inputString, NULL);
				exit(0);
			}
			write(client_sockfd, inputString, 1000);
		}
	}
	close(client_sockfd);
}
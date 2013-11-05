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

		char **tempArray = (char**)malloc(10 * sizeof(char*));
		int size = 0;
		while(commandString != NULL)
		{
			tempArray[size] = commandString;
			printf("%s\n", tempArray[size]);
			commandString = strtok (NULL, " ");
			size++;
		}
		while(size < 10)
		{
			tempArray[size] = NULL;
			size++;
		}

		char *const argv[10] = {tempArray[0], tempArray[1], tempArray[2], tempArray[3], tempArray[4], tempArray[5], tempArray[6], tempArray[7]};

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
				execv(path, argv);
				exit(0);
			}
		}
	}
	close(client_sockfd);
}
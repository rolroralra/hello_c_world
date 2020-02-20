# hello_c_world

```bash
$ gcc -o serverMain serverMain.c
$ gcc -o clientMain clientMain.c

$ ./serverMain 127.0.0.1 7777

$ ./clientMain 127.0.0.1 7777
```


# serverMain.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char* argv[]) {

	int servSockfd;
	int clntSockfd;

	struct sockaddr_in servAddr;
	struct sockaddr_in clntAddr;
	char buffer[BUFSIZE];
	int clntLen;
	int recvLen;
	char clntIp[16] = {0x00,};
	char clntPort[6] = {0x00,};


	if ((servSockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket FAILED!");	
		exit(1);
	}


	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	//servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[2]));


	if (bind(servSockfd, (struct sockaddr*) &servAddr, sizeof(servAddr)) == -1) {
		printf("ip: %s, port: %s\n", argv[1], argv[2]);
		perror("bind FAILED!");
		exit(2);
	}


	if (listen(servSockfd, 5) == -1) {
		perror("listen FAILED!");
		exit(3);
	}


	while (1) {
		clntLen = sizeof(clntAddr);

		if ((clntSockfd = accept(servSockfd, (struct sockaddr*) &clntAddr, &clntLen)) == -1) {
			perror("accept FAILED!");
			exit(4);
		}


		inet_ntop(AF_INET, &(clntAddr.sin_addr), clntIp, INET_ADDRSTRLEN);
		printf("CONNECTED :: ip[%d], port[%d]\n", clntAddr.sin_addr.s_addr, clntAddr.sin_port);
		printf("CONNECTED :: ip[%s], port[%d]\n", clntIp, (int) ntohs(clntAddr.sin_port));


		while (1) {
			if ((recvLen = recv(clntSockfd, buffer, BUFSIZE - 1, 0)) == -1) {
				perror("recv FAILED!");
				exit(5);
			}


			if (recvLen == 0) {
				break;
			}


			buffer[recvLen] = 0x00;
			printf("Received: %s\n", buffer);
		}

		close(clntSockfd);
	}
}
```


# clientMain.c
```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char* argv[]) {

	int sockfd;

	struct sockaddr_in servAddr;
	char buffer[BUFSIZE];
	int recvLen;


	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket FAILED!");	
		exit(1);
	}


	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.s_addr = htonl(inet_addr(argv[1]));
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));


	if (connect(sockfd, (struct sockaddr*) &servAddr, sizeof(servAddr)) == -1) {
		perror("connect FAILED!");
		exit(2);
	}


	while (1) {
		fgets(buffer, BUFSIZE, stdin);

		if (send(sockfd, buffer, strlen(buffer), 0) != strlen(buffer)) {
			perror("send FAILED!");
			exit(3);
		}
	}

	close(sockfd);

}
```

This is made in 여의도.

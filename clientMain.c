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

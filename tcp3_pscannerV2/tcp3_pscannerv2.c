/*
 * This program uses TCP 3-way handshake to check if a port is open.
 * IPv4/6 compatible.
 * Not a very stealthy approach, only for API practice.
 */

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

/* #define MAX 100 */

void usage(char *name)
{
	printf("Usage:\n");
	printf("%s [IP | HOSTNAME]\n", name);
	printf("Ipv4/6 compatible.\n");
	exit(0);
}

int main (int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in client_addr;
	struct sockaddr_in *sinp;
	struct addrinfo hint;
	struct addrinfo *result, *rp;
	char buff[100];
	int counter1, result1;
	char *addrp;
	
	if ((argc < 2) || (argc > 2))
		usage(argv[0]);

	/* initializing address structure for destination IP */
	memset(&client_addr, 0, sizeof(struct sockaddr_in));

	/* initializing addrinfo structure list */
	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;

	/* Converting the input address to network presentation */
	result1 = inet_pton(AF_INET, argv[1], &client_addr.sin_addr);
	if(result1 < 0 )
		perror("Error converting IP to hostname.\n");
	else
		/* try convert input to IP */
		if(getaddrinfo(argv[1], 0, &hint, &result) != 0)
			printf("Error getaddrinfo()\n");
	
	
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		if (rp->ai_family == AF_INET) {
			printf("IPV4 detected\n");
			sinp = (struct sockaddr_in *) rp->ai_addr;
			addrp = inet_ntop(AF_INET, &sinp->sin_addr, buff, 100);
			printf("Address %s\n", addrp?addrp:"unknow");
		} else if (rp->ai_family == AF_INET6)
		       printf("IPV6 detected\n");	
		
	}
		//if ((sockfd = socket(rp->ai_family, SOCK_STREAM, 0)) == -1)
			//perror("Error in socket()");
		//	continue;

		//for (counter1 = 0; counter1 < 65536; counter1++) {	
			/* Setting the port number to scan */
		//	client_addr.sin_port = htons(counter1);

		//	if((connect(sockfd, (struct sockaddr *) &client_addr, 
		//			sizeof(struct sockaddr_in)) == 0))
		//		printf("Port %d is open\n", counter1);
			//else
			//fflush(stdout);
			/* printf("Port %d is close\n", client_addr.sin_port);*/

		//	close(sockfd);
		//}
	//fflush(stdout);
	freeaddrinfo(result);

	return 0;
}


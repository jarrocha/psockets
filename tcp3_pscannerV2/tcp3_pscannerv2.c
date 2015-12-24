/*
 * This program uses TCP 3-way handshake to check if a port is open.
 * IPv4/6 compatible.
 * Notes:
 *	-Not a very stealthy approach, only for API practice.
 */

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "hresolver.h"

#define DATA 100 

void usage(char *name)
{
	printf("Usage:\n");
	printf("%s [IP | HOSTNAME]\n", name);
	printf("Ipv4/6 compatible.\n");
	exit(0);
}



int main(int argc, char **argv)
{
	//int sockfd;
	struct sockaddr_in client_addr4;
	struct sockaddr_in6 client_addr6;
	struct addrinfo hint;
	struct addrinfo *rp;
	struct str_host hresult;
	char buff[DATA];
	int resv4, resv6;
	
	/* variables for IP storing */
	struct sockaddr_in **str_in;
	struct sockaddr_in6 **str_in6;

	if ((argc < 2) || (argc > 2))
		usage(argv[0]);

	/* initializing address structure for destination IP */
	memset(&client_addr4, 0, sizeof(struct sockaddr_in));
	memset(&client_addr6, 0, sizeof(struct sockaddr_in6));
	memset(&hint, 0, sizeof(struct addrinfo));

	/* initializing addrinfo structure list */
	hint.ai_flags = AI_ALL;
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;

	/* Converting the input address to network presentation */
	resv4 = inet_pton(AF_INET, argv[1], &client_addr4.sin_addr);
	resv6 = inet_pton(AF_INET6, argv[1], &client_addr6.sin6_addr);
	if (resv4 == 0  && resv6 == 0) {
		if (rsolvhost(&hint, &hresult, argv[1]) == 1) {
			printf("Error resolving hostnames\n");
			exit(1);
		}
		printf("\n\n# of IPv4 address found: %d\n", hresult.num_host4);
		printf("\n\n# of IPv6 address found: %d\n", hresult.num_host6);

		free_rsolvhost(hresult.ptr_host);
	} else if (resv4 == 1)
		printf("IPv4 address given\n");
	else
		printf("IPv6 address given\n");
		
	/* Allocating memory for storing IP arrays */
	str_in = malloc(sizeof(str_in) * hresult.num_host4);
	str_in6 = malloc(sizeof(str_in6) * hresult.num_host6);

	/* Store IPs in arrays */
	get_ip(&hint, &hresult, str_in, str_in6);
	
	

	
	
	
	
	
	
	//if ((sockfd = socket(rp->ai_family, SOCK_STREAM, 0)) == -1)
			//perror("Error in socket()");
		//	continue;

		//for (counter1 = 0; counter1 < 65536; counter1++) {	
			/* Setting the port number to scan */
		//	client_addr4.sin_port = htons(counter1);

		//	if((connect(sockfd, (struct sockaddr *) &client_addr4, 
		//			sizeof(struct sockaddr_in)) == 0))
		//		printf("Port %d is open\n", counter1);
			//else
			//fflush(stdout);
			//printf("Port %d is close\n", client_addr4.sin_port);

		//	close(sockfd);
		//}
	//fflush(stdout);
	//freeaddrinfo(rp);

	return 0;
}


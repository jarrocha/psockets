/*
 * This program uses TCP 3-way handshake to check if a port is open.
 */

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


void usage(char *name)
{
	printf("Usage:\n");
	printf("%s [IP | HOSTNAME]\n", name);
	exit(0);
}

int main (int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in cl_addr;
	int counter;
	
	if ((argc < 2) || (argc > 2))
		usage(argv[0]);
	
	memset(&(cl_addr.sin_zero), 0, sizeof(cl_addr.sin_zero));
	/* Specifying the address family */
	cl_addr.sin_family = AF_INET;
	
	/* Converting the input address to network presentation */
	if(inet_pton(AF_INET, argv[1], &cl_addr.sin_addr))
			printf("pton conversion successful\n");

	for (counter = 0; counter < 65536; counter++) {	
		/* Setting the port number to scan */
		cl_addr.sin_port = htons(counter);

		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			perror("Error in socket()");
		if((connect(sockfd, (struct sockaddr *) &cl_addr, 
			sizeof(struct sockaddr_in)) == 0))
			printf("Port[%d]: open\n", counter);
		else
			fflush(stdout);

		close(sockfd);
	}
	fflush(stdout);

	return 0;
}

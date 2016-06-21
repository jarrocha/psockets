/*
 * A simple server to demonstrate TCP sockets implementation. The server
 * takes input from server and shows dump of the data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DATA 50
#define S_PORT 7890

void dump(char *, const unsigned int);

int main(void)
{
	/* server socket variables */
	int sockfd, new_sockfd;
	struct sockaddr_in host_addr, client_addr;
	socklen_t sin_size = sizeof(struct sockaddr_in);
	int recv_length = 1, yes = 1;
	
	/* data holding */
	char buffer[1024];

	/* defining listening socket descriptor */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		perror("in socket");
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
			== -1)
		perror("setting socket option SO_REUSEADDR");
	
	/* initializing server address socket structure */
	host_addr.sin_family = AF_INET;		/* host byte order */
	host_addr.sin_port = htons(S_PORT);	/* port, network byte order */
	host_addr.sin_addr.s_addr = 0;		/* use host address */

	/* zero the rest of struct */
	memset(&(host_addr.sin_zero), '\0', sizeof(host_addr.sin_zero));

	/* bind socket to IP address */ 
	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr))
			== -1)
		perror("binding to socket");
	
	/* listen on socket */ 
	if (listen(sockfd, 5) == -1)
		perror("listening on socket");
	
	/* main procedure */
	while (1) {
		new_sockfd = accept(sockfd, (struct sockaddr *) &client_addr,
				 &sin_size);
		if (new_sockfd == -1)
			perror("accepting connection");
		printf("server: got connection from %s port %d\n",
			inet_ntoa(client_addr.sin_addr),
			ntohs(client_addr.sin_port));
		recv_length = recv(new_sockfd, &buffer, DATA, 0);

		while (recv_length > 0) {
			printf("RECV: %d bytes\n", recv_length);
			dump(buffer, recv_length);
			recv_length = recv(new_sockfd, &buffer, DATA, 0);
		}
	
		close(new_sockfd);
	}
	return 0;
}

/* dumps raw memory in hex byte and printable split format */
void dump(char *data_buffer, const unsigned int length)
{
	unsigned char byte;
	unsigned int i, j;
	
	for (i = 0; i < length; i++) {
		byte = data_buffer[i];
		printf("%02x ", data_buffer[i]);  /* displays byte in hex */
		
		if (((i % 16) == 15) || (i == length - 1)) {
			for (j = 0; j < 15 - (i % 16); j++)
				printf("   ");
			printf("| ");
			/* display printable bytes from line */
			for (j = (i - (i % 16)); j <= i; j++) {
				byte = data_buffer[j];
				/* outside printable char range */
				if ((byte > 31) && (byte < 127))
					printf("%c", byte);
				else
					printf(".");
			}
			/* end of the dump line (each line 16 bytes) */
			printf("\n");
		}
	}
}

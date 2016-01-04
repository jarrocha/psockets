/*
 * Implementation of resolver function
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "hresolver.h"

int rsolvhost(struct addrinfo *type, struct str_host *hresult, char *argv)
{
	int i, j, status;
	char buff4[INET_ADDRSTRLEN];
	char buff6[INET6_ADDRSTRLEN];
	const char *pchar;
	struct addrinfo *rp;
	struct sockaddr_in *p4;
	struct sockaddr_in6 *p6;
	struct str_host *result = hresult;

	if ((status = getaddrinfo(argv, 0, type, &rp)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
	

	result->ptr_host = rp;
	for(i = 0, j = 0; rp != NULL; rp = rp->ai_next) {
		if(rp->ai_family == AF_INET) {
			result->num_host4 = ++i;
			p4 = (struct sockaddr_in *) rp->ai_addr;
			pchar = inet_ntop(AF_INET, &p4->sin_addr, buff4,
					INET_ADDRSTRLEN);
			printf("Address: %s\n", pchar?pchar:"unknown");
		} else if (rp->ai_family == AF_INET6) {
			result->num_host6 = ++j;
			p6 = (struct sockaddr_in6 *) rp->ai_addr;
			pchar = inet_ntop(AF_INET6, &p6->sin6_addr, buff6,
					INET6_ADDRSTRLEN);
			printf("Address: %s\n", pchar?pchar:"unknown");
		}
	}

	return 0;
}

void free_rsolvhost(struct addrinfo *rp)
{
	freeaddrinfo(rp);
}

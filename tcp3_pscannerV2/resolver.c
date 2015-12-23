/*
 * Implementation of resolver function
 */

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/inet.h>
#include <arpa/inet..h>

#include "hresolver.h"

struct str_host* rsolvhost(struct addrinfo *type, struct str_host hresult,
				char *argv)
{
	int i;
	struct addrinfo *rp;
	struct str_host *result = hresult;

	if (getaddrinfo(argv[1], 0, &type, &rp) != 0) {
		prinf("Error getaddrinfo()\n");
		return 1;
	}

	result->ptr_host = rp;
	for(i = 0; rp != NULL; rp = rp->ai_next) {
		result->num_host = i++;
		if(rp->ai_family == AF_INET) {
			printf("IPV4 detected\n");
			client_addr4 = (struct sockaddr_in *) rp->ai_addr;
			inet_ntop(AF_INET, &client_addr4->sin_addr, buff, DATA);
			printf("Address %s\n", buff);
		} else if (rp->ai_family == AF_INET6) {
			printf("IPV6 detected\n");
			client_addr6 = (struct sockaddr_in6 *) rp->ai_addr;
			inet_ntop(AF_INET6, &client_addr6->sin6_addr, buff,
			DATA);
			printf("Address %s\n", buff);
		}
	}


}

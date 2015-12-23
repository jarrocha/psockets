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
#include <arpa/inet.h>

#include "hresolver.h"

bool rsolvhost(struct addrinfo *type, struct str_host *hresult,
				char *argv)
{
	int i, j;
	struct addrinfo *rp;
	struct str_host *result = hresult;

	if (getaddrinfo(argv, 0, type, &rp) != 0) {
		printf("Error getaddrinfo()\n");
		return 1;
	}
	

	result->ptr_host = rp;
	for(i = 1, j = 1; rp != NULL; rp = rp->ai_next) {
		if(rp->ai_family == AF_INET) {
			result->num_host4 = i++;
			//client_addr4 = (struct sockaddr_in *) rp->ai_addr;
			//inet_ntop(AF_INET, &client_addr4->sin_addr, buff, DATA);
			//printf("Address %s\n", buff);
		} else if (rp->ai_family == AF_INET6) {
			result->num_host6 = j++;
			//client_addr6 = (struct sockaddr_in6 *) rp->ai_addr;
			//inet_ntop(AF_INET6, &client_addr6->sin6_addr, buff,
			//DATA);
			//printf("Address %s\n", buff);
		}
	}

	return 0;

}

void free_rsolvhost(struct addrinfo *rp)
{
	/*
	struct addrinfo *tmp1 = rp, *tmp2;
	
	while (tmp1 != NULL) {
		tmp2 = tmp1->ai_next;
		free(tmp1);
		freeaddrinfo(tmp1);
		tmp1 = tmp2;
	}
	*/
	freeaddrinfo(rp);
}

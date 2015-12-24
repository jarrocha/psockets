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

void get_ip(struct addrinfo *type, struct str_host *hresult, 
		struct sockaddr_in **str_in, struct sockaddr_in6 **str_in6)
{
	int i, j;
	struct addrinfo *tmp = hresult->ptr_host;
	
	for (i = 0, j = 0; tmp != NULL; tmp = tmp->ai_next, i++, j++) {
		if (tmp->ai_family == AF_INET)
			*(str_in + i) = (struct sockaddr_in *) tmp->ai_addr;
		else
			*(str_in6 + j) = (struct sockaddr_in6 *) tmp->ai_addr;
	}
}

int rsolvhost(struct addrinfo *type, struct str_host *hresult, char *argv)
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
		if(rp->ai_family == AF_INET)
			result->num_host4 = i++;
		else if (rp->ai_family == AF_INET6)
			result->num_host6 = j++;
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

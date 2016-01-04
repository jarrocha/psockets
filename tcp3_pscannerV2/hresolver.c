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

void get_ip(struct addrinfo *type, struct str_host *hresult, 
		struct sockaddr_in **str_in, struct sockaddr_in6 **str_in6)
{
	int i, j;
	const char *data;
	char buff4[INET_ADDRSTRLEN];
	char buff6[INET6_ADDRSTRLEN];
	struct addrinfo *tmp = hresult->ptr_host;
	
	for (i = 0, j = 0; tmp != NULL; tmp = tmp->ai_next) {
		if (tmp->ai_family == AF_INET){
			*(str_in + i++) = (struct sockaddr_in *) tmp->ai_addr;
			data = inet_ntop(AF_INET, &(*str_in + i)->sin_addr, 
					buff4, INET_ADDRSTRLEN);
			printf("Address: %s\n", data?data:"unknown");
			//printf("ipv4# %d\n", i);
	/*	} else if (tmp->ai_family == AF_INET6) {
			*(str_in6 + j++) = (struct sockaddr_in6 *) tmp->ai_addr;
			inet_ntop(AF_INET6, &(*str_in6 + i)->sin6_addr, buff6,
				INET6_ADDRSTRLEN);
			printf("Address: %s\n", buff6?buff6:"unknown");
			//printf("ipv6# %d\n", j);
	*/}
	}
	
}

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

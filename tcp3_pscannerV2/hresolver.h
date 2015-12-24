/*
 * Interface for resolver function
 */

#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

struct str_host {
	int num_host4;
	int num_host6;
	struct addrinfo *ptr_host;
};

int rsolvhost(struct addrinfo *, struct str_host *, char *argv );

void get_ip(struct addrinfo *, struct str_host *, struct sockaddr_in **,
		struct sockaddr_in6 **);

void free_rsolvhost(struct addrinfo *);

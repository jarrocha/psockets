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

bool rsolvhost(struct addrinfo *, struct str_host *, char *argv );

void free_rsolvhost(struct addrinfo *);

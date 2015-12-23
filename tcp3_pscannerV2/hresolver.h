/*
 * Interface for resolver function
 */

#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/inet.h>
#include <arpa/inet.h>

struct str_rsolvhost {
	int num_host;
	struct addrinfo *ptr_host;
}

struct str_rsolvhost* fnc_rsolvhost(struct addrinfo *type, 
					struct addrinfo *result, char *argv );

#include <stdio.h>                                                              
#include <stdlib.h>                                                             
#include <string.h>                                                             
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>                                                         
#include <sys/types.h>                                                          
#include <netinet/in.h>                                                         
#include <arpa/inet.h>                                                          
                                                                                
#define MAX 2000

ssize_t recv_msg(u_int, char *, size_t);
ssize_t send_msg(u_int, char *);
void get_http_ipv4(const u_int *, struct sockaddr_in *, char *);
void get_http_ipv6(const u_int *, struct sockaddr_in6 *, char *);
void free_rsolvhost(struct addrinfo *);
void err_msg(const char *);
void usage(const char *);

int main(int argc, char *argv[])
{
	u_int trx_fd;
	u_int resv4;
	u_int resv6;
	u_int status = 1;
	char buffer[MAX];
	struct addrinfo hints;		/* DNS resolution processing */
	struct addrinfo *rs;		/* DNS results */
	struct sockaddr_in svr_addr4;
	struct sockaddr_in *pr;
	struct sockaddr_in6 svr_addr6;
	struct timeval tt_wait;		/* used for recv() timeout */
	tt_wait.tv_sec = 3;		/* 3 secs timeout */
	tt_wait.tv_usec= 500000;

	if(argc != 2) {
		usage(argv[0]);
		exit(0);
	}

	/* initializing addrinfo structure list */                              
        hints.ai_flags = AF_INET;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = 0;
        hints.ai_addrlen = 0;
        hints.ai_canonname = NULL;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;

	/* get IP info */
	resv4 = inet_pton(AF_INET, argv[1], &svr_addr4.sin_addr);
	resv6 = inet_pton(AF_INET6, argv[1], &svr_addr6.sin6_addr);

	/* entering http server presets in structure */
	svr_addr4.sin_port =  htons((in_port_t) 80);
	svr_addr4.sin_family = AF_INET;
	svr_addr6.sin6_port =  htons((in_port_t) 80);
	svr_addr6.sin6_family = AF_INET6;
	sprintf(buffer, "GET / HTTP/1.1\r\n\r\n");
		
	/* checking for either IPv4 or IPv6 entered input */
	while (status == 1) {
		if(resv4 > 0) {
			if((trx_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
				err_msg("Error creating socket");
			if ((setsockopt(trx_fd, SOL_SOCKET, SO_RCVTIMEO,
					(struct timeval *)&tt_wait, 
					sizeof(struct timeval))) == -1)
				err_msg("setsockopt()");
			/* get out of the loop */
			status = 0;
			get_http_ipv4(&trx_fd, &svr_addr4, buffer);

		} else if(resv6 > 0) {
			if((trx_fd = socket(AF_INET6, SOCK_STREAM, 0)) == -1)
				err_msg("Error creating socket");
			if ((setsockopt(trx_fd, SOL_SOCKET, SO_RCVTIMEO, 
					(struct timeval *)&tt_wait, 
					sizeof(struct timeval))) == -1)
				err_msg("setsockopt()");

			/* get out of the loop */
			status = 0;
			get_http_ipv6(&trx_fd, &svr_addr6, buffer);

		} else {
			if ((getaddrinfo(argv[1], 0, &hints, &rs)) != 0)
				err_msg(gai_strerror(status));
			resv4 = 1;
			for(; rs != NULL; rs = rs->ai_next){
				if(rs->ai_family == AF_INET) {
					/* typecasting result and assigning */
					pr =  (struct sockaddr_in *)rs->ai_addr;
					svr_addr4.sin_addr = pr->sin_addr;
					break;
				}
			}
			/* freeing allocated struct rs memory */
			freeaddrinfo(rs);		
		}
	}
	shutdown(trx_fd, SHUT_WR);
	exit(EXIT_SUCCESS);
}

/* process transmission with IPv6 */ 
void get_http_ipv6(const u_int *fd, struct sockaddr_in6 *saddr6, char *buff)
{
	u_int nrecv;
	u_int trx_fd = *fd;
	struct sockaddr_in6 addr6;

	memcpy(&addr6, saddr6, sizeof(*saddr6));
	if(connect(trx_fd, (struct sockaddr *) &addr6, sizeof(addr6)) == -1)
		err_msg("connect()");
	else {
		printf("Connect success\n");
		
		/* start transmission */
		send_msg(trx_fd, buff);
		while((nrecv = recv_msg(trx_fd, buff, MAX)) > 0) {
			printf("%s", buff);
			memset(buff, 0 , MAX);
		}
	}
}

/* process transmission with IPv4 */ 
void get_http_ipv4(const u_int *fd, struct sockaddr_in *saddr4, char *buff)
{
	u_int nrecv;
	u_int trx_fd = *fd;
	struct sockaddr_in addr4;

	memcpy(&addr4, saddr4, sizeof(*saddr4));
	if(connect(trx_fd, (struct sockaddr *) &addr4, sizeof(addr4)) == -1)
		err_msg("connect()");
	else {
		printf("Connect success\n");

		/* start transmission */
		send_msg(trx_fd, buff);
		while((nrecv = recv_msg(trx_fd, buff, MAX)) > 0) {
			printf("%s", buff);
			memset(buff, 0 , MAX);
		}
	}
}

/* receive msg function */
ssize_t recv_msg(u_int fd, char *usr_buff, size_t nbytes)
{	
        size_t nleft = nbytes;
        ssize_t nrecv; 
	char *recv_buff = usr_buff;
	
	while (nleft > 0) {
		if ((nrecv = recv(fd, recv_buff, nleft, 0)) < 0) {
			if (errno == EINTR)
                                nrecv = 0;
			else
                                err_msg("No more data in recv()");
				return 0;
		} else if ((nrecv == 0 ) || strstr(recv_buff, "\r\n"))
			break;
		
		nleft -= nrecv;
                recv_buff += nrecv;
        }
        return nrecv;
} 

/* send msg function */                                                         
ssize_t send_msg(u_int fd, char *buff)
{                                                                               
        size_t nleft = strlen(buff);
        ssize_t nsend = 0;

        /* send content */
        while (nleft > 0) {
                if ((nsend = send(fd, buff, nleft, 0)) <= 0 ) {
                        if ( nsend < 0 && errno == EINTR)
                                nsend = 0;
                        else
                                err_msg("error with send()");
                }
                nleft -= nsend;
                buff += nsend;
        }
	return nleft;
}

/* exit error messages */
void err_msg(const char *msg)
{
	fprintf(stderr, "%s: %s\n", strerror(errno), msg);
	exit(EXIT_FAILURE);
}

/* how to use the program */
void usage(const char *arg)
{
	printf("%s [IP ADDRESS | HOSTNAME]\n", arg);
	printf("This program obtains the /index.html of the address given.\n");
	printf("Supports IPv4, IPv6, and DNS resolution\n");
	exit(EXIT_SUCCESS);
}

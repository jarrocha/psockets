#include <stdio.h>                                                              
#include <stdlib.h>                                                             
#include <string.h>                                                             
#include <errno.h>                                                              
#include <sys/socket.h>                                                         
#include <sys/types.h>                                                          
#include <netinet/in.h>                                                         
#include <arpa/inet.h>

#define MAX 2000

/* receive msg function */                                                      
ssize_t recv_msg(int fd, char *usr_buff, size_t nbytes)                         
{                                                                               
        size_t nleft = nbytes;                                                  
        ssize_t nrecv;                                                          
        ssize_t msg_peek;
	u_char *recv_buff = usr_buff;                                           
	
	while (nleft > 0) {
		if ((nrecv = recv(fd, recv_buff, nleft, 0)) < 0) {              
                        if (errno == EINTR)                                     
                                nrecv = 0;                                      
                        else                                                    
                                printf("recv() error\n");
		} else if ((nrecv == 0 ) || strstr(recv_buff, "\r\n"))
			break;
		
		nleft -= nrecv;                                                 
                recv_buff += nrecv;                                             
        }
        return nrecv;                                                  
                                                                                
} 

/* send msg function */                                                         
size_t send_msg(int fd, char *buff)                                             
{                                                                               
        size_t nleft = strlen(buff);                                            
        ssize_t nsend = 0;                                                      
                                                                                
        /* send content */                                                      
        printf("Bytes to send: %d\n", (int) nleft);                             
        while (nleft > 0) {                                                     
                if ((nsend = send(fd, buff, nleft, 0)) <= 0 ) {                 
                        if ( nsend < 0 && errno == EINTR)                       
                                nsend = 0;                                      
                        else                                                    
                                printf("error with send()");                 
                }                                                               
                nleft -= nsend;                                                 
                buff += nsend;                                                  
        }                                                                       
        if (nleft == 0)                                                         
                printf("Send success!!\n");                                     
        return nleft;                                                           
}

int main(int argc, char *argv[])
{
	u_int conn_fd;
	u_int trx_fd;
	u_int nrecv;
	u_int svr_ip_addr;
	u_char buffer[MAX];
	struct sockaddr_in svr_addr;


	if(argc != 2) {
		/* Make this better */
		printf("Bad usage\n");
		exit(0);
	}

	if((inet_pton(AF_INET, argv[1], &svr_addr.sin_addr)) <= 0 )
		printf("Bad IP\n");
	svr_addr.sin_port =  htons((in_port_t) 80);
	svr_addr.sin_family = AF_INET;
	sprintf(buffer, "GET /index.html HTTP/1.1\r\n\r\n");


	if((trx_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		printf("Error creating socket\n");
	
	if(connect(trx_fd, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) 
			== -1)
		printf("Error connecting socket\n");
	else {
		printf("Connect success\n");
		
		send_msg(trx_fd, buffer);
		while((nrecv = recv_msg(trx_fd, buffer, MAX)) > 0) {
			printf("%s", buffer);
			memset(buffer, 0 , MAX);
		}
			
	}
	return 0;
}

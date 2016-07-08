/*
 *  psniff: packet sniffer implemented with raw sockets.
 *
 *  Copyright (C) 2016 Jaime Arrocha
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>                                                              
#include <stdlib.h>                                                             
#include <string.h>                                                             
#include <errno.h>                                                              
#include <sys/socket.h>                                                         
#include <sys/types.h>                                                          
#include <netinet/in.h>                                                         
#include <arpa/inet.h> 



#define BUFFER 576

struct dhcp_pckt {
	unsigned char op;
	unsigned char htype;
	unsigned char hlen;
	unsigned char hops;
	unsigned int xid;
	unsigned short secs;
	unsigned short flags;
	unsigned int ciaddr;
	unsigned int yiaddr;
	unsigned int siaddr;
	unsigned int giaddr;
	unsigned char chaddr[16];
	unsigned char file[128];
};


int main(int argc, char *argv[])
{
	u_int trx_fd;
	u_int cl_ipaddr;
	struct sockaddr_in claddr;
	
	if((inet_pton(AF_INET, argv[1], &claddr.sin_addr) <= 0 )
			printf("Bad IP\n");
	

	trx_fd = socket(AF_INET, SOCK_DGRAM, 0);








}



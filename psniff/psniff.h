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

#define BUFF 65536
#define ETH_P_ALL 0x0003

/* procpkt.c */
void proc_packet(const u_char *, u_int); 


/*
 * Ethernet frame header
 */
#define ETHER_ADDR_LEN 6
#define ETHER_HDR_LEN 14

struct ether_hdr {
	unsigned char dst_addr[ETHER_ADDR_LEN];
	unsigned char src_addr[ETHER_ADDR_LEN];
	unsigned short type;
};


/*
 * IP header
 * Note: Supports Little-Endian only
 */

#define IP_HDR_LEN 20
struct ip_hdr {
	unsigned char ver_ihl;
	unsigned char tos;
	unsigned short len;
	unsigned short id;
	unsigned short frag_off;
	unsigned char ttl;
	unsigned char protocol;
	unsigned short check;
	unsigned int src_addr;
	unsigned int dst_addr;
};


/*
 * TCP
 * Note: Supports Little-Endian only
 */

struct tcp_hdr {
	unsigned short src_port;
	unsigned short dst_port;
	unsigned int seq;
	unsigned int ack;
	unsigned char reserved:4; 
	unsigned char offset:4;
	unsigned char flags;
#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
#define TCP_PUSH 0x08
#define TCP_ACK 0x10
#define TCP_URG 0x20
	unsigned short window;
	unsigned short checksum;
	unsigned short urgent;
};


/*
 * UDP Header
 */

struct udp_hdr {
	unsigned short src_port;
	unsigned short dst_port;
	unsigned short len;
	unsigned short checksum;
};


/*
 * ICMP Header
 */

struct icmphdr
{
  	unsigned char type;
  	unsigned char code;
  	unsigned short checksum;
	unsigned int gateway;
};

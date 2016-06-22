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

#include "psniff.h"

void praw(const u_char *, u_int);
void decode_ethernet(const u_char *);
u_int decode_ip(const u_char *);
void decode_icmp(const u_char *);
void decode_tcp(const u_char *);
void decode_udp(const u_char *);

/* takes buffer, typecast it to an ethernet header and dissects it */
void decode_ethernet(const u_char *data)
{
	int i;
	const struct ether_hdr *ehdr;

	ehdr = (const struct ether_hdr *) data;
	printf("[[ Layer 2 :: Ethernet Header ]]\n");
	printf("[ Source: %02x", ehdr->src_addr[0]);
	for (i = 1; i < ETHER_ADDR_LEN; i++)
		printf(":%02x", ehdr->src_addr[i]);

	printf("\tDestination: %02x", ehdr->dst_addr[0]);
	for (i = 1; i < ETHER_ADDR_LEN; i++)
		printf(":%02x", ehdr->dst_addr[i]);
	printf("\tType: %hu ]\n", ehdr->type);
}

/* takes buffer, typecast it to an ip header and dissects it */
u_int decode_ip(const u_char *data)
{

	const struct ip_hdr *iphdr;

	iphdr = (const struct ip_hdr *) data;
	printf("\t((  Layer 3 ::: IP Header  ))\n");
	printf("\t( Source:%s\t", 
			inet_ntoa(*(struct in_addr *) &iphdr->src_addr));
	printf("Dest: %s )\n", 
			inet_ntoa(*(struct in_addr *) &iphdr->dst_addr));
	printf("\t( Type: %u\t", (u_int) iphdr->protocol);
	printf("\tID: %hu\tLength: %hu )\n", ntohs(iphdr->id), 
			                     ntohs(iphdr->len));
	return (u_int) iphdr->protocol;
}

/* takes buffer, typecast it to an ip header and dissects it */
void decode_tcp(const u_char *data)
{
	u_int header_size;
	const struct tcp_hdr *thdr;

	thdr = (const struct tcp_hdr *) data;

}

/* main processing function */
void proc_packet(const u_char *data, u_int size)
{	
	unsigned int prot;
	
	/* print captured bytes */
	printf("Bytes Captured = [ %d ]\n", size);
	
	/* print L2 information */
	decode_ethernet(data);

	/* obtain IP protocol number for further decoding */
	prot = decode_ip(data + ETHER_HDR_LEN);
	/*
	switch(prot)
	{
		case 1:
			decode_icmp(data);
			break;
		case 6:
			decode_tcp(data + ETHER_HDR_LEN + 
				   sizeof(struct ip_hdr);
			break;
		case 17:
			decode_udp(data);
			break;
		default:
			break;
	}
	*/


	/* print raw encapsulated data */
	praw(data, size);
				

}

/* print trasmitted raw data */                                                
void praw(const u_char *buffer, u_int length)                              
{                                                                               
        u_char byte;                                                            
        u_int i, j;                                                             
                                                                                
        for (i = 0; i < length; i++) {                                          
                byte = buffer[i];                                          
                printf("%02x ", buffer[i]);  /* displays byte in hex */    
                                                                                
                if (((i % 16) == 15) || (i == length - 1)) {                    
                        for (j = 0; j < 15 - (i % 16); j++)                     
                                printf("   ");                                  
                        printf("    | ");                                       
                        /* display printable bytes from line */                 
                        for (j = (i - (i % 16)); j <= i; j++) {                 
                                byte = buffer[j];                          
                                /* outside printable char range */              
                                if ((byte > 31) && (byte < 127))                
                                        printf("%c", byte);                     
                                else                                            
                                        printf(".");                            
                        }                                                       
                        /* end of the dump line (each line 16 bytes) */         
                        printf("\n\n");                                         
                }                                                               
        }                                                                       
}

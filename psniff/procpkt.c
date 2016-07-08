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
void decode_ip(const u_char *, struct ip_stat *);
void decode_icmp(const u_char *);
u_int decode_tcp(const u_char *);
u_short decode_udp(const u_char *);

/* takes buffer, typecast it to an ethernet header and dissects it */
void decode_ethernet(const u_char *data)
{
	int i;
	const struct ether_hdr *ehdr;

	ehdr = (const struct ether_hdr *) data;
	printf("[ Src MAC:  %02x", ehdr->src_addr[0]);
	for (i = 1; i < ETHER_ADDR_LEN; i++)
		printf(":%02x", ehdr->src_addr[i]);

	printf("\t\t]\n[ Dst MAC:  %02x", ehdr->dst_addr[0]);
	for (i = 1; i < ETHER_ADDR_LEN; i++)
		printf(":%02x", ehdr->dst_addr[i]);
	printf("\tType:  %hu ]\n", ehdr->type);
}

/* takes buffer, typecast it to an ip header and dissects it */
void decode_ip(const u_char *data, struct ip_stat *ipstat)
{

	const struct ip_hdr *iphdr;

	iphdr = (const struct ip_hdr *) data;
	ipstat->ip_len = ntohs(iphdr->len);
	ipstat->ip_prot = (u_int) iphdr->protocol;

	printf("    [ Src IP:  %s \t]\n", 
			inet_ntoa(*(struct in_addr *) &iphdr->src_addr));
	printf("    [ Dst IP:  %s \t]\n", 
			inet_ntoa(*(struct in_addr *) &iphdr->dst_addr));
	printf("    [ Protocol: %u   ", (u_int) iphdr->protocol);
	printf("ID: %hu   Length: %hu ]\n", ntohs(iphdr->id), 
			                     ntohs(iphdr->len));
}

/* takes buffer, typecast it to an ip header and dissects it */
u_int decode_tcp(const u_char *data)
{
	const struct tcp_hdr *thdr;

	thdr = (const struct tcp_hdr *) data;
	u_int hdr_size = 4 * thdr->offset;

	printf("\t[ Src Port: %hu ] ", ntohs(thdr->src_port));
	printf("\t[ DST Port: %hu ]\n", ntohs(thdr->dst_port));
	printf("\t[ Seq #: %u   Ack# : %u ]\n", ntohl(thdr->seq),
						ntohl(thdr->ack));
	printf("\t[ Header Size: %u   Flags: ", hdr_size);
	if(thdr->flags & TCP_FIN)
		printf("FIN ");
	if(thdr->flags & TCP_SYN)
		printf("SYN ");
	if(thdr->flags & TCP_RST)
		printf("RST ");
	if(thdr->flags & TCP_PUSH)
		printf("PUSH ");
	if(thdr->flags & TCP_ACK)
		printf("ACK ");
	if(thdr->flags & TCP_URG)
		printf("URG ");
	printf("\n");

	return hdr_size;
}


/* takes buffer, typecast it to an ip header and dissects it */
void decode_icmp(const u_char *data)
{
	const struct icmp_hdr *ihdr;

	ihdr = (const struct icmp_hdr *) data;
	
	printf("\t[ Type: %u ] ", (u_int) ihdr->type);
	printf("\t[ Code: %u ]\n", (u_int) ihdr->code);
	printf("\t[ Gateway: %u]\n", ntohs(ihdr->gateway));
}


/* takes buffer, typecast it to an ip header and dissects it */
u_short decode_udp(const u_char *data)
{
	const struct udp_hdr *uhdr;

	uhdr = (const struct udp_hdr *) data;
	u_short udr_size = uhdr->len;

	printf("\t[ Src Port: %hu ] ", ntohs(uhdr->src_port));
	printf("\t[ DST Port: %hu ]\n", ntohs(uhdr->dst_port));
	printf("\t[ Header Size: %u]\n", ntohs(udr_size));

	return udr_size;
}


/* main processing function */
void proc_packet(const u_char *data, u_int size)
{	
	struct ip_stat istat;
	u_int data_offs;
	u_int tcp_hdr_sz;
	u_int udp_hdr_sz;
	
	/* print captured bytes */
	printf("\n\nBytes Captured = [ %d ]\n", size);
	
	/* print L2 information */
	decode_ethernet(data);

	/* obtain IP protocol number for further decoding */
	decode_ip(data + ETHER_HDR_LEN, &istat);
	
	switch(istat.ip_prot) {
	case 1:
		decode_icmp(data + ETHER_HDR_LEN + istat.ip_len);
		data_offs = ETHER_HDR_LEN + istat.ip_len;
		break;
	case 6:
		tcp_hdr_sz = decode_tcp(data + ETHER_HDR_LEN + istat.ip_len);
		data_offs = ETHER_HDR_LEN + istat.ip_len + tcp_hdr_sz;
		break;
	case 17:
		udp_hdr_sz = decode_udp(data + ETHER_HDR_LEN + istat.ip_len);
		data_offs = ETHER_HDR_LEN + istat.ip_len + udp_hdr_sz;
		break;
	default:
		break;
	}


	/* print raw encapsulated data */
	praw(data + data_offs, size);
				

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
                        printf("\n");                                         
                }                                                               
        }                                                                       
}

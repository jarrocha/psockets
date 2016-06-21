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


void usage(char *);
int matches(const char *, const char *);
void praw(u_char *, const u_int);
void err_msg(const char *);

int main(int argc, char *argv[])
{
	u_int rcv_bytes;
	u_int trx_fd;
	u_char buffer[BUFF];
	struct sockaddr_in cl_addr;
	socklen_t cl_size;

	if (argc != 2)
		usage(argv[0]);
	
	
	if((trx_fd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
		err_msg("Error on socket()");
	if((setsockopt(trx_fd, SOL_SOCKET, SO_BINDTODEVICE, argv[1], 
			strlen(argv[1]) + 1)) == -1)
		err_msg("Error on setsockopt()");
	
	for(;;) {
		if((rcv_bytes = recvfrom(trx_fd, buffer, BUFF, 0, 
					 (struct sockaddr *) &cl_addr, 
					 &cl_size)) == -1)
			err_msg("Error on recvfrom()");
		printf("Bytes Captured = [ %d ]\n", rcv_bytes);
		praw(buffer, rcv_bytes);
	}

	return 0;
}

/* printf trasmitted raw data */
void praw(u_char *data_buffer, const u_int length)
{
	u_char byte;
	u_int i, j;
	
	for (i = 0; i < length; i++) {
		byte = data_buffer[i];
		printf("%02x ", data_buffer[i]);  /* displays byte in hex */
		
		if (((i % 16) == 15) || (i == length - 1)) {
			for (j = 0; j < 15 - (i % 16); j++)
				printf("   ");
			printf("    | ");
			/* display printable bytes from line */
			for (j = (i - (i % 16)); j <= i; j++) {
				byte = data_buffer[j];
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

/* exit error messages */
void err_msg(const char *msg)
{
        fprintf(stderr, "%s: %s\n", msg, strerror(errno));
        exit(EXIT_FAILURE);
}

/* compares two strings */
int matches(const char* cmd, const char* pattern)
{
	int len = strlen(cmd);

	if(len > strlen(pattern))
		return -1;
	return memcmp(pattern, cmd, len);
}

/* usage function */
void usage(char *cmd)
{
	printf("Usage: %s [INTERFACE]\n", cmd);
	exit(EXIT_SUCCESS);
}

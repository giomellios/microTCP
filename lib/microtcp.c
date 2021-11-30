/*
 * microtcp, a lightweight implementation of TCP for teaching,
 * and academic purposes.
 *
 * Copyright (C) 2015-2017  Manolis Surligas <surligas@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "microtcp.h"
#include "../utils/crc32.h"
#include <unistd.h>

microtcp_sock_t
microtcp_socket (int domain, int type, int protocol)
{

 	microtcp_sock_t s1;
	if ( ( s1.sd = socket(domain ,type,protocol ) ) == -1){
		perror("opening TCP listening socket\n");
	}

	s1.state = UKNOWN;
	return s1;   

  /* Your code here */
}

int
microtcp_bind (microtcp_sock_t *socket, const struct sockaddr *address,
               socklen_t address_len)
{
	if(bind(socket->sd,(struct sockaddr*)sin,address_len)==-1){
		perror("TCP bind\n");
		exit(EXIT_FAILURE);
	}
}

int
microtcp_connect (microtcp_sock_t *socket, const struct sockaddr *address,
                  socklen_t address_len)
{
        microtcp_header_t send;
        microtcp_header_t *receive=(microtcp_header_t*)malloc(sizeof(microtcp_header_t));
        
        
        //initializing the header (to be sent to client)of microtcp to start the 3-way handshake
        send.seq_number=htonl(rand());
        send.ack_number=0;
        //send.control=
        send.window=0;
        send.data_len=0;
        send.future_use0=0;
        send.future_use1=0;
        send.future_use2=0;
        send.checksum=0;


        if(sendto(socket->sd,&send,sizeof(microtcp_header_t),0,address,address_len)==-1){
                perror("Send first packet of 3-way handshake error:");
                socket->state=INVALID;
                return -1;
        }

        if(recvfrom(socket->sd,receive,sizeof(microtcp_header_t),0,address,&address_len)==-1){
                perror("Receive from server error:");
                socket->state=INVALID;
                return -1;
        }

}

int
microtcp_accept (microtcp_sock_t *socket, struct sockaddr *address,
                 socklen_t address_len)
{
	int sock_accept;
	socket->state=ESTABLISHED;
	socket->ack_number=0;
	return 0;
}

int

microtcp_shutdown (microtcp_sock_t *socket, int how)
{

 //*** client sends FIN pocket ***//
 //*** server receivs FIN - sends ACK - state = CLOSING_BY_PEER ***//
 //*** client receivs ACK - state = CLOSING_BY_HOST ***//
 //*** server sends FIN pocket ***//
 //*** client receivs FIN pocket - sends ACK pocket - state = CLOSED ***//
 //*** server receivs ACK pocket - state = CLOSED ***//

 microtcp_sock_t s2;


 if(how == 0){
	 s2.state = CLOSING_BY_PEER;
 }
 else if(how == 1){
	s2.state = CLOSING_BY_HOST;
 }

 free(socket);
 s2.state = CLOSED;




  /* Your code here */
}

ssize_t
microtcp_send (microtcp_sock_t *socket, const void *buffer, size_t length,
               int flags)
{
        ssize_t bytes_returned;
        if(bytes_returned=send(socket->sd,buffer,length,flags)==-1){
                perror("TCP send:\n");
                return -1;
        }
        return bytes_returned;
}

ssize_t
microtcp_recv (microtcp_sock_t *socket, void *buffer, size_t length, int flags)
{
	
        ssize_t bytes_returned;
        if(bytes_returned=recv(socket->sd,buffer,length,flags)==-1){
                perror("TCP receive\n");
                return -1;
        }
        return bytes_returned;
}



/*
 * vasa_udp_client.c
 *
 *  Created on: 31 jul 2014
 *      Author: cag01
 */

#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include <unistd.h>

#include "vasa_udp_client.h"

struct sockaddr_in vasa_addr;
int vasa_socket;
int broadcast_on=0;

void init_vasa_address(uint32_t port) {
	 /*
	   * Setup connection to Vasa
	   */
	  memset(&vasa_addr, 0, sizeof(vasa_addr));

	  /* Set connection properties */
	  vasa_addr.sin_family = AF_INET;
	  vasa_addr.sin_addr.s_addr = inet_addr(VASA_IP);
	  vasa_addr.sin_port = htons(port);
}

void init_address(uint32_t port, char* addr) {
	 /*
	   * Setup connection to Vasa
	   */
	  memset(&vasa_addr, 0, sizeof(vasa_addr));

	  /* Set connection properties */
	  vasa_addr.sin_family = AF_INET;
	  vasa_addr.sin_addr.s_addr = inet_addr(addr); //not working: htonl(INADDR_ANY);
	  vasa_addr.sin_port = htons(port);
}

void init_broadcast_address(uint32_t port) {
	 /*
	   * Setup connection to Vasa
	   */
	  memset(&vasa_addr, 0, sizeof(vasa_addr));

	  /* Set connection properties */
	  vasa_addr.sin_family = AF_INET;
	  vasa_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	  vasa_addr.sin_port = htons(port);
	  broadcast_on = 1;
}


int8_t init_vasa_socket_udp () {
	/* Open a socket */
	if ((vasa_socket = socket(AF_INET, SOCK_DGRAM , 0)) < 0)
	{
	  fprintf(stderr,"Could not create socket for communication with Vasa. Exiting.\n");
	  return -1;
	}
	setsockopt(vasa_socket, SOL_SOCKET, SO_BROADCAST, &broadcast_on, sizeof(broadcast_on));
	return 0;
}

int8_t init_vasa_socket_tcp () {
	/* Open a socket */
	if ((vasa_socket = socket(AF_INET, SOCK_STREAM , 0)) < 0)
	{
	  fprintf(stderr,"Could not create socket for communication with Vasa. Exiting.\n");
	  return -1;
	}
	if (connect(vasa_socket, (struct sockaddr*)&vasa_addr, sizeof(struct sockaddr)) < 0)
	{
		  fprintf(stderr,"Could not connect to Vasa. Exiting.\n");
		  return -1;
	}
	return 0;
}



int8_t sendto_vasa (message_t *msg) {
	if (sendto(vasa_socket, (void*)msg, sizeof(message_t), 0, (struct sockaddr*)&vasa_addr, sizeof(struct sockaddr)) < 0)
		{
		perror("sendto");
		  fprintf(stderr,"Failed to send message to Vasa... Could be that the message size is too big. Exiting.\n");
		  return -1;
		}
	return 0;
}



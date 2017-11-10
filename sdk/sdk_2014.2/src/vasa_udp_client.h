/*
 * vasa_udp_client.h
 *
 *  Created on: 31 jul 2014
 *      Author: cag01
 */

#ifndef VASA_UDP_CLIENT_H_
#define VASA_UDP_CLIENT_H_


#define VASA_IP "192.168.1.100" //127.0.0.1"
#define VASA_FV_PORT 18004
#define VASA_BV_PORT 18003

#include "message.h"

void init_vasa_address(uint32_t port);
void init_address(uint32_t port, char* addr);
void init_broadcast_address(uint32_t port);
int8_t init_vasa_socket_udp ();
int8_t init_vasa_socket_tcp ();
int8_t sendto_vasa (message_t *msg);


#endif /* VASA_UDP_CLIENT_H_ */

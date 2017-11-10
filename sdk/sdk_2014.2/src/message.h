/*
 * message.h
 *
 *  Created on: 31 jul 2014
 *      Author: cag01
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>

/********************************/
/* The theoretical limit for TCP is 65535 bytes */
/* The limit for IP is 1500 bytes */
/* Use something smaller (note that the whole struct represents the message)! */
//#define PIXELS_PER_FRAME 480  //(1500-sizeof(int32_t)/3 > 960/2
//#define PIXELS_PER_FRAME 320*4  //(1500-sizeof(int32_t)/3 > 960/2
//#define PIXELS_PER_FRAME 240*4  //(1500-sizeof(int32_t)/3 > 960/2
#define PIXELS_PER_FRAME 480*3

/* This is a generic message which should be used for sending data between nodes */
/* Add valid field? Boolean telling whether the message contains garbage or not */
//typedef struct
//{
//	uint8_t r,g,b;
//} rgb_t;

typedef struct
{
	unsigned int cam_id;
	unsigned int seq_no;
	char pixels[PIXELS_PER_FRAME]; //rgb_t for rgb24, now uint32_t for rgb32
} message_t;
/************************************/


#endif /* MESSAGE_H_ */

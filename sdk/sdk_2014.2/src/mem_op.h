/*
 * mem_op.h
 *
 *  Created on: 17 apr 2015
 *      Author: cag01
 */

#ifndef MEM_OP_H_
#define MEM_OP_H_

typedef struct
{
	unsigned int w;
	unsigned int h;
	unsigned int bpp; //bytes per pixel
	char* data;
} image_t;

int mem_copy(char* dest_ptr, unsigned int BaseAddress, unsigned int map_len); //use mem_to_im instead
int mem_to_im(image_t *dest, unsigned int BaseAddress);
int im_to_mem(unsigned int BaseAddress, image_t *src);
int im_alloc(image_t* im);
void im_free(image_t* im);

#endif /* MEM_OP_H_ */

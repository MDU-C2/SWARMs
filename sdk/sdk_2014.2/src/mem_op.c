/*
 * mem_op.c
 *
 *  Created on: 17 apr 2015
 *      Author: cag01
 */

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "mem_op.h"

int mem_copy(char* dest_ptr, unsigned int BaseAddress, unsigned int map_len)
{
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	char* mem_base = (char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	if(mem_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	memcpy(dest_ptr, mem_base, map_len);

	munmap((void *)mem_base, map_len);
	close(fd);
	return 0;
}

int mem_to_im(image_t *dest, unsigned int BaseAddress)
{
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	int map_len = dest->bpp*dest->h*dest->w;

	char* mem_base = (char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	if(mem_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	memcpy(dest->data, mem_base, map_len);

	munmap((void *)mem_base, map_len);
	close(fd);

	return 0;
}

int im_to_mem(unsigned int BaseAddress, image_t *src)
{
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	int map_len = src->bpp*src->h*src->w;

	char* mem_base = (char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	if(mem_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	memcpy(mem_base, src->data, map_len);

	munmap((void *)mem_base, map_len);
	close(fd);
	return 0;
}

int im_alloc(image_t* im)
{

	im->data = (unsigned char *) malloc(im->bpp*im->h*im->w);
	if (im->data == NULL)
	{
		perror("Allocating memory failed (im_alloc).\n");
		return -1;
	}
	return 0;
}

void im_free(image_t* im)
{
	free(im->data);
}

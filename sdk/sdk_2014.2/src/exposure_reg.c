/*
 * exposure_reg.c
 *
 *  Created on: 30 jun 2015
 *      Author: cag01
 */

#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include "hw_base.h"
#include "exposure_reg.h"


int expreg_read(uint32_t *pos, uint32_t *neg, uint32_t base_addr, uint32_t high_addr)
{
//	uint32_t tmp;

	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return -1;
	}

	int map_len = high_addr-base_addr;


	unsigned char* base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)base_addr);

	if(base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	*pos = REG_READ(base,EXPREG1_OFFSET);
	*neg = REG_READ(base,EXPREG2_OFFSET);

	munmap((void *)base, map_len);
	close(fd);
	return 0;
}

int expreg_set_th(uint8_t th, uint32_t base_addr, uint32_t high_addr)
{
//	uint32_t tmp;

	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return -1;
	}

	int map_len = high_addr-base_addr;


	unsigned char* base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)base_addr);

	if(base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	REG_WRITE_T(base,EXPREG0_OFFSET,th,uint8_t);

	munmap((void *)base, map_len);
	close(fd);
	return 0;
}

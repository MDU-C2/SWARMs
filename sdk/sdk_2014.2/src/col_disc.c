/*
 * col_disc.c
 *
 *  Created on: 25 jun 2015
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
#include "col_disc.h"

void coldisc_th_print(coldisc_th_t *th)
{
	printf("Coldisc thresholds:\n");
	printf("B: %d\n",th->B);
    printf("W: %d\n",th->W);
    printf("S: %d\n",th->S);
    printf("H1: %d\n",th->H1);
    printf("H2: %d\n",th->H2);
    printf("H3: %d\n",th->H3);
    printf("H4: %d\n",th->H4);
    printf("H5: %d\n",th->H5);
    printf("H6: %d\n",th->H6);
}

int coldisc_th_read(coldisc_th_t *th, uint32_t base_addr, uint32_t high_addr)
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

	*th = REG_READ_T(base,REG0_OFFSET,coldisc_th_t);

//	tmp = REG_READ(base,REG0_OFFSET);
//	th->B = tmp & 0xFF;
//	th->W = (tmp >> 8) & 0xFF;
//	th->S = (tmp >> 16) & 0xFF;
//	th->H1 = (tmp >> 24) & 0xFF;
//
//	tmp = REG_READ(base,REG1_OFFSET);
//	th->H2 = tmp & 0xFF;
//	th->H3 = (tmp >> 8) & 0xFF;
//	th->H4 = (tmp >> 16) & 0xFF;
//	th->H5 = (tmp >> 24) & 0xFF;
//
//	tmp = REG_READ(base,REG2_OFFSET);
//	th->H6 = tmp & 0xFF;

	munmap((void *)base, map_len);
	close(fd);
	return 0;
}

int coldisc_th_write(coldisc_th_t *th, uint32_t base_addr, uint32_t high_addr)
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

	REG_WRITE_T(base,REG0_OFFSET,*th,coldisc_th_t);

//	tmp = (th->H1 << 24) + (th->S << 16) + (th->W <<8) + th->B;
//	REG_WRITE(base,REG0_OFFSET,tmp);
//
//	tmp = (th->H5 << 24) + (th->H4 << 16) + (th->H3 <<8) + th->H2;
//	REG_WRITE(base,REG1_OFFSET,tmp);
//
//	tmp = th->H6;
//	REG_WRITE(base,REG2_OFFSET,tmp);

	munmap((void *)base, map_len);
	close(fd);
	return 0;
}

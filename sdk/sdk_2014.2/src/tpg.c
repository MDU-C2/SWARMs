/*
 * tpg.c
 *
 *  Created on:
 *      Author: Carl
 */

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <inttypes.h>
#include "tpg.h"
#include "hw_base.h"

void TPG_Start(uint32_t BaseAddress)
{
	int map_len = 0xFFF;

	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return ;
	}

	unsigned char* TPG_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);
	if(TPG_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

//	REG_WRITE(TPG_base, TPG_PATTERN_CONTROL, PTRN_RED);
//	REG_WRITE(TPG_base, TPG_PATTERN_CONTROL, PTRN_TARTAN);
//	REG_WRITE(TPG_base, TPG_PATTERN_CONTROL, PTRN_COLOR_BARS);
	REG_WRITE(TPG_base, TPG_CONTROL, TPG_CTL_EN_MASK|TPG_CTL_RUE_MASK);
	munmap((void *)TPG_base, map_len);
	close(fd);

	return;
}

void TPG_Stop(uint32_t BaseAddress)
{
	int map_len = 0xFFF;
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return ;
	}

	unsigned char* TPG_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);

	if(TPG_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	REG_WRITE(TPG_base, TPG_CONTROL, 0);
	munmap((void *)TPG_base, map_len);
	close(fd);

	return;
}

int TPG_Registers(uint32_t uBaseAddr)
{

	int map_len = 0xFF;
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	unsigned char* TPG_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)uBaseAddr);

	if(TPG_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

   // Partial Register Dump
   printf( "TPG - Partial Register Dump (uBaseAddr = 0x%08X):\n\r", uBaseAddr );
   printf( "\t TPG_CONTROL         = 0x%08X\n\r", REG_READ(TPG_base, TPG_CONTROL));
   printf( "\t TPG_STATUS          = 0x%08X\n\r", REG_READ(TPG_base, TPG_STATUS));
   printf( "\t TPG_ERROR           = 0x%08X\n\r", REG_READ(TPG_base, TPG_ERROR) );
   printf( "\t TPG_IRQ_EN          = 0x%08X\n\r", REG_READ(TPG_base, TPG_IRQ_EN) );
   printf( "\t TPG_VERSION         = 0x%08X\n\r", REG_READ(TPG_base, TPG_VERSION) );
   printf( "\t TPG_ACTIVE_SIZE     = 0x%08X\n\r", REG_READ(TPG_base, TPG_ACTIVE_SIZE) );
//   printf( "\t TPG_PATTERN_CONTROL = 0x%08X\n\r", REG_READ(TPG_base, TPG_PATTERN_CONTROL) );
//   printf( "\t TPG_MOTION_SPEED    = 0x%08X\n\r", REG_READ(TPG_base, TPG_MOTION_SPEED ) );
//#define TPG_CROSS_HAIRS 			0x108
//#define TPG_ZPLATE_HOR_CONTROL 		0x10C
//#define TPG_ZPLATE_VER_CONTROL 		0x110
//#define TPG_BOX_SIZE 				0x114
//#define TPG_BOX_COLOR 				0x118
//#define TPG_STUCK_PIXEL_THRESH 		0x11C
//#define TPG_NOISE_GAIN 				0x120

   munmap((void *)TPG_base, map_len);
   close(fd);
   return 0;
}

int TPG_Errors(uint32_t BaseAdress)
{
   uint32_t Status, Errors;

	int map_len = 0xFF;
	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return -1;
	}

	unsigned char* TPG_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAdress);

	if(TPG_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
   	}

   Status = REG_READ(TPG_base, TPG_STATUS );
   Errors = REG_READ(TPG_base, TPG_ERROR );
   printf( "AXI_TPG - Checking Error Flags\n\r" );


   if ( Status & 0x00010000 ) //bit 16)
	   printf( "\tTPG_STATUS - ERROR\n\r" );
   else
	   printf("No errors found!\n");


   if ( Errors & 0x00000001 ) //bit 0
	  printf( "\tS2TPG_ERROR - SLAVE_EOL_EARLY\n\r" );
   if ( Errors & 0x0000002 ) //bit 1
	  printf( "\tS2TPG_ERROR - SLAVE_EOL_LATE\n\r" );
   if ( Errors & 0x00000004 ) //bit 2
	  printf( "\tS2TPG_ERROR - SLAVE_SOF_EARLY\n\r" );
   if ( Errors & 0x0000008 ) //bit 3
	  printf( "\tS2TPG_ERROR - SLAVE_SOF_LATE\n\r" );


   munmap((void *)TPG_base, map_len);
   close(fd);
   return 0;
}

void TPG_Set_Size(uint32_t BaseAddress, uint16_t h_size, uint16_t v_size)
{
	int map_len = 0xFFF;
	uint32_t reg_data = (v_size << 16) + h_size;

	int fd = open( "/dev/mem", (O_RDWR | O_SYNC));
	if(fd < 0)
	{
		printf("Failed to open /dev/mem!\n");
		return ;
	}

	unsigned char* TPG_base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)BaseAddress);
	if(TPG_base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return;
	}

	REG_WRITE(TPG_base, TPG_ACTIVE_SIZE, reg_data);
	munmap((void *)TPG_base, map_len);
	close(fd);

	return;
}


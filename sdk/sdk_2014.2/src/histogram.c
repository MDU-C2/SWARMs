/*
 * histogram.c
 *
 *  Created on: 25 jun 2017
 *      Author: cag01
 */

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include "histogram.h"

int histogram_ctrl_set(uint32_t ctrl_reg)
{
//	uint32_t tmp;

	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return -1;
	}

	int map_len = 4;


	unsigned char* base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)HISTOGRAM_BASEADDR);

	if(base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	REG_WRITE(base,HISTOGRAM_CTRL_OFFSET,ctrl_reg);

	if (munmap((void *)base, map_len) == -1)
		{
			perror("Unapping memory for absolute memory access failed.\n");
			close(fd);
			return -1;
		}
	close(fd);
	return 0;
}

int histogram_ctrl_get(uint32_t* ctrl_reg)
{
//	uint32_t tmp;

	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return -1;
	}

	int map_len = 4;


	unsigned char* base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)HISTOGRAM_BASEADDR);

	if(base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	*ctrl_reg = REG_READ(base,HISTOGRAM_CTRL_OFFSET);

	if (munmap((void *)base, map_len) == -1)
		{
			perror("Unapping memory for absolute memory access failed.\n");
			close(fd);
			return -1;
		}
	close(fd);
	return 0;
}

int histogram_get(uint32_t *hist)
{
	int fd = open( "/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return -1;
	}

	int map_len = HISTOGRAM_HIGHADDR-HISTOGRAM_BASEADDR;


	unsigned char* base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)HISTOGRAM_BASEADDR);

	if(base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}

	uint32_t offset=HISTOGRAM_BUCKET1_OFFSET;
	uint8_t i=0;
	for (i=0; i<HISTOGRAM_NBUCKETS; i++, offset+=4)
		hist[i] = REG_READ(base,offset);

	if (munmap((void *)base, map_len) == -1)
	{
		perror("Unapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

void histogram_print(uint32_t *hist)
{

	uint8_t i;
	for (i=0; i<HISTOGRAM_NBUCKETS; i++)
	{
		printf("\t%d",hist[i]);


	}
	printf("\n");
}

void histogram_print_norm(float *hist_norm)
{

	uint8_t i;
	for (i=0; i<HISTOGRAM_NBUCKETS; i++)
	{
		printf("\t%f",hist_norm[i]);


	}
	printf("\n");
}

void histogram_normalize(uint32_t *hist_in, float *hist_norm)
{
	uint32_t max = hist_in[0];
	uint8_t i;
	for (i=1;i<HISTOGRAM_NBUCKETS;i++)
		if (hist_in[i]>max)
			max=hist_in[i];
	for (i=0;i<HISTOGRAM_NBUCKETS;i++)
		hist_norm[i] = (float)hist_in[i]/(float)max;
}

float histogram_stats(uint32_t *hist)
{
	uint8_t i;
	float sum = hist[0];
	float sum_w = 0; //the first weight is 0
	uint32_t mode = hist[0];
	uint16_t mode_bucket = 0;

	//calculate sums and mode
	for (i=1;i<HISTOGRAM_NBUCKETS;i++)
	{
		sum+=hist[i];
		sum_w+=(hist[i]*i);
		if (hist[i]>mode)
		{
			mode=hist[i];
			mode_bucket = i;
		}
	}

	float mean = sum_w/sum;

	return mean;

//	//find the median
//	float median_tmp = sum/2;
//	uint8_t median;
//	float sum_tmp = 0;
//	for(i=0;i<HISTOGRAM_NBUCKETS;i++)
//	{
//		sum_tmp+=hist[i];
//		if (median_tmp < sum_tmp)
//		{
//			median = i;
//			break;
//		}
//	}
//
//
//	printf("Mean: %f, Median %d, Mode bucket %d, Mode %d, sum %f\n",mean,median,mode_bucket,mode,sum);
//
//
//	//Normalized
//	float hist_norm[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//	for (i=0;i<HISTOGRAM_NBUCKETS;i++)
//		hist_norm[i] = (float)hist[i]/(float)mode;
//
//	sum=0;
//	float sum_sq=0;
//	for (i=0; i<HISTOGRAM_NBUCKETS; i++)
//		{
//			sum+=hist_norm[i];
//			sum_sq+=(hist_norm[i]*hist_norm[i]);
//		}
//
//	histogram_print_norm(hist_norm);
//	printf("Area: %f, Area_sq: %f\n",sum,sum_sq);
}

float histogram_exp(float mean)
{
	if (mean < HISTOGRAM_EXP_TH_LOW)
		return HISTOGRAM_EXP_STEP;
	if (mean > HISTOGRAM_EXP_TH_HIGH)
		return 1/HISTOGRAM_EXP_STEP;
	return 1.0;
}

//-----------------------------------------------------

int histogram_enable()
{
	int fd = open("/dev/mem", O_RDWR | O_SYNC);
	if(fd < 0)
	{
		perror("Failed to open /dev/mem!\n");
		return -1;
	}

	int map_len = 4; //only ctrl register

	unsigned char* base = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)HISTOGRAM_BASEADDR);

	if(base == MAP_FAILED)
	{
		perror("Mapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}


	uint32_t ctrl_reg = REG_READ(base, HISTOGRAM_CTRL_OFFSET);
	SET_BIT(ctrl_reg,HISTOGRAM_CTRL_ENABLE);
	REG_WRITE(base, HISTOGRAM_CTRL_OFFSET, ctrl_reg);

	if (munmap((void *)base, map_len) == -1)
	{
		perror("Unapping memory for absolute memory access failed.\n");
		close(fd);
		return -1;
	}
	close(fd);

//	printf("pin %d set high\n",pin);
	return 0;
}

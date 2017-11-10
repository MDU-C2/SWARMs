/*
 * Copyright (c) 2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
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
#include <stdbool.h>
#include "hw_base.h"
#include "VDMA.h"
#include "axi_gpio.h"
#include "vasa_udp_client.h"
#include <time.h>
#include "mem_op.h"
//#include <cv.h>
//#include <highgui.h>

int kbhit()
{
    // timeout structure passed into select
    struct timeval tv;
    // fd_set passed into select
    fd_set fds;
    // Set up the timeout.  here we can wait for 1 second
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    // Zero out the fd_set - make sure it's pristine
    FD_ZERO(&fds);
    // Set the FD that we want to read
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    // select takes the last file descriptor value + 1 in the fdset to check,
    // the fdset for reads, writes, and errors.  We are only passing in reads.
    // the last parameter is the timeout.  select will return if an FD is ready or
    // the timeout has occurred
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    // return 0 if STDIN is not ready to be read.
    return FD_ISSET(STDIN_FILENO, &fds);
}

int main(int argc, char *argv[])
{
	uint32_t pin_data = 0;
	uint32_t pin_dir = 0x0;
	uint32_t w = 640;
	uint32_t h = 480;
	uint8_t bpp = 4;
	uint32_t port = VASA_FV_PORT;
	bool loop = true;

	uint32_t vdma_addr = VDMA0_BASEADDR;
	uint32_t th_c = 100;
	uint32_t th_e = 1000;
	unsigned char id = 0;

	image_t im;
	im.h = h;
	im.w = w;
	im.bpp = 4;
	if (im_alloc(&im) < 0) return -1;

//	int ret;
//	char c;

//	CvSize size;
//	size.height = h;
//	size.width = w;
//	IplImage* img = cvCreateImage(size, IPL_DEPTH_8U, bpp); //Creates an image header and allocates the image data

	// get the image data
//	unsigned int height,width,step,channels, i, j, k;
//	char *data;
//	height    = img->height;
//	width     = img->width;
//	step      = img->widthStep;
//	channels  = img->nChannels;
//	data      = img->imageData;

	if (argc>1)
		switch(atoi(argv[1])) {
		case 0:
			vdma_addr = VDMA0_BASEADDR; //right
			printf("right image\n");
			break;
		case 1:
			vdma_addr = VDMA1_BASEADDR; //left
			printf("left image\n");
			break;
		case 2:
			vdma_addr = VDMA2_BASEADDR; //disp
			printf("disparity image\n");
			break;
		case 3:
			vdma_addr = VDMA3_BASEADDR; //harris
			printf("harris image\n");
			break;
		}
	if (argc>2) th_c = atoi(argv[2]);
	if (argc>3) th_e = atoi(argv[3]);

	set_harris_th(true, th_c);
	set_harris_th(false, th_e);

	init_vasa_address(port);
	if (init_vasa_socket_udp () == -1) return -1;

	axi_gpio_init_dir();
	axi_gpio_init_val(&pin_data); //cam i2c high, resets low

	SetSize (w,h,bpp);

	InitFB(FB0_ADDR); //write DEADBEEFs
	InitFB(FB1_ADDR);
	InitFB(FB2_ADDR);

	ResetVDMA(vdma_addr);
	SetupVDMAs2mm(vdma_addr,FB0_ADDR,FB1_ADDR,FB2_ADDR);

	cam_reset(&pin_data);
	init_cams_sync(&pin_data,&pin_dir, SIOD_0, SIOC_0, SIOD_1, SIOC_1);
	fpga_clk_reset(&pin_data);
	usleep(100);

	fpga_pipe_reset(&pin_data); //release fgpa soft reset



	usleep(200000);
	printf("Starting stream\n...");




	do {
//		SendImage(FB0_ADDR);

		mem_copy(im.data, FB0_ADDR, w*h*bpp);
		SendImage_mem(im.data,id);

//		mem_copy(img->imageData, FB0_ADDR, w*h*bpp); //copy memory

		// invert the image
//		for(i=0;i<height;i++) for(j=0;j<width;j++) for(k=0;k<channels;k++)
//			if (k != 3)
//				data[i*step+j*channels+k]=255-data[i*step+j*channels+k];

//		SendImage_mem(img->imageData, id);

		if (kbhit()) loop = false;
	} while (loop);


	// save the image
//	cvSaveImage("result.bmp", img, NULL);
	SaveImage_mem(&im,"saved.bmp");

	im_free(&im);
//	cvReleaseImage(&img);
	StopVDMA(vdma_addr);
	printf("done\n");
    return 0;
}

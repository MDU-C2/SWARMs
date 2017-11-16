/*
 * VDMA.h
 *
 *  Created on: 23 maj 2013
 *      Author: Emil
 */

#ifndef VDMA_H_
#define VDMA_H_

//Register addresses for VDMA can be found in hw_base.h

#define MAP_SIZE 					4096UL
#define MAP_MASK 					(MAP_SIZE - 1)

//these are now set by SetSize
//#define WIDTH 						640 //960
//#define HEIGHT 						480 //540
//#define WIDTH 						320
//#define HEIGHT 						120
//#define WIDTH 						1920
//#define HEIGHT 						1374
#include <stdint.h>
#include "mem_op.h"
//added
void SendTcpImageFromServer(int socket, char* filename);
void SendTcpImgDataFromServer(int socket, unsigned char *memo_data, char *name, unsigned long mem_size);
int receive_tcp_image_client(int socket);
int InitTcpImgDataServer(unsigned char *memory_data, char *name, unsigned long mem_size); //, char *filename1);
//int InitTcpServer( char *filename0, char *filename1);
int init_tcp_client();
//end added
void SetSize (uint32_t w, uint32_t h, uint32_t bpp);
int CheckAndClearErrors(uint32_t BaseAdress);
int DumpRegisters(uint32_t uBaseAddr);
void InitFB(off_t FB_addr, uint16_t width, uint16_t height, uint16_t bpp);
int CheckResults(uint32_t BaseAddress, int memLen, int resolution);
int SendImage(uint32_t BaseAddress, uint16_t width, uint16_t height, uint16_t bpp);
int SendImage_mem(image_t* img, char id);
int SaveImage(uint32_t BaseAddress, char* filename, uint16_t width, uint16_t height, uint16_t bpp, uint8_t scale);
int SaveImage32(uint32_t BaseAddress, char* filename);
int SaveImage_mem(image_t* im, char* filename);
int LoadImage_mem(image_t* im, char* filename);
//int SaveImageDisp(uint32_t BaseAddress, char* filename, uint8_t scale);
//int SaveImageHarris(uint32_t BaseAddress, char* filename, uint32_t corner_threshold, uint32_t edge_threshold, uint8_t ov, uint32_t BaseAddress2);
uint32_t GetImageHeight(char* filename);
uint32_t GetImageWidth(char* filename);
//int LoadImage(uint32_t BaseAddress, char* filename);
//int LoadImageStereo(uint32_t BaseAddress, char* filenameL, char* filenameR);
void SetupVDMAmm2s(uint32_t BaseAddress, uint16_t width, uint16_t height, uint16_t bpp, uint32_t frameAddress1, uint32_t frameAddress2, uint32_t frameAddress3);
void SetupVDMAs2mm(uint32_t BaseAddress, uint16_t width, uint16_t height, uint16_t bpp, uint32_t frameAddress1, uint32_t frameAddress2, uint32_t frameAddress3);
void StopVDMA(uint32_t BaseAddres);
void ResetVDMA(uint32_t BaseAddress);

#endif /* VDMA_H_ */

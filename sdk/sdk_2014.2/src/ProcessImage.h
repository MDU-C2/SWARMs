
#ifndef PROCESSIMAGE_H_
#define PROCESSIMAGE_H_

#include "stdio.h"
#include "jpeglib.h"
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef long LONG;

int ConvertImage(unsigned char *img, unsigned char *img_arr, int image_width, int image_height, int bpp);
int ConvertImageData(unsigned char *img, unsigned char *img_arr, int image_width, int image_height, int bpp,  unsigned char * mem, unsigned long *mem_size);

#endif

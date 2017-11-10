
#ifndef PROCESSIMAGE_H_
#define PROCESSIMAGE_H_

#include "stdio.h"
#include "jpeglib.h"
//#include <fstream>
#include <stdlib.h>
//#include <inttypes.h>
#include <stdint.h>

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef long LONG;


int ConvertImage(unsigned char *img, unsigned char *img_arr, int image_width, int image_height, int bpp);

#endif

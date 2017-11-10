/*
 * histogram.h
 *
 *  Created on: 25 jun 2017
 *      Author: cag01
 */

#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "hw_base.h"

#define HISTOGRAM_BASEADDR 0x43C00000
#define HISTOGRAM_HIGHADDR 0x43C0FFFF
#define HISTOGRAM_CTRL_OFFSET 0x00
#define HISTOGRAM_BUCKET1_OFFSET 0x04
#define HISTOGRAM_BUCKET2_OFFSET 0x08
#define HISTOGRAM_BUCKET3_OFFSET 0x0C
#define HISTOGRAM_BUCKET4_OFFSET 0x10
#define HISTOGRAM_BUCKET5_OFFSET 0x14
#define HISTOGRAM_BUCKET6_OFFSET 0x18
#define HISTOGRAM_BUCKET7_OFFSET 0x1C
#define HISTOGRAM_BUCKET8_OFFSET 0x20
#define HISTOGRAM_BUCKET9_OFFSET 0x24
#define HISTOGRAM_BUCKET10_OFFSET 0x28
#define HISTOGRAM_BUCKET11_OFFSET 0x2C
#define HISTOGRAM_BUCKET12_OFFSET 0x30
#define HISTOGRAM_BUCKET13_OFFSET 0x34
#define HISTOGRAM_BUCKET14_OFFSET 0x38
#define HISTOGRAM_BUCKET15_OFFSET 0x3C
#define HISTOGRAM_BUCKET16_OFFSET 0x40

#define HISTOGRAM_NBUCKETS 16
#define HISTOGRAM_CTRL_DEFAULT 0xA0029 //enable, 10th frame, 10th pixel,
#define HISTOGRAM_EXP_TH_LOW 7
#define HISTOGRAM_EXP_TH_HIGH 8
#define HISTOGRAM_EXP_STEP 1.1


//ctrl register
#define HISTOGRAM_CTRL_ENABLE 0
#define HISTOGRAM_CTRL_VALID 1
#define HISTOGRAM_CTRL_FRAME_CNT_LOW 2
#define HISTOGRAM_CTRL_FRAME_CNT_HIGH 15
#define HISTOGRAM_CTRL_PIXEL_CNT_LOW 16
#define HITOSTGRAM_CTRL_PIXEL_CNT_HIGH 31

int histogram_ctrl_set(uint32_t ctrl_reg);
int histogram_ctrl_get(uint32_t* ctrl_reg);
int histogram_get(uint32_t *hist);
void histogram_normalize(uint32_t *hist_in, float *hist_norm);
float histogram_stats(uint32_t *hist);
float histogram_exp(float mean);

void histogram_print(uint32_t *hist);
void histogram_print_norm(float *hist_norm);


#endif /* HISTOGRAM_H_ */

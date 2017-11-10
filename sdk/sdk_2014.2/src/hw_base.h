/******************************************************************************
 *
 * (c) Copyright 2013 Xilinx, Inc. All rights reserved.
 *
 * This file contains confidential and proprietary information of Xilinx, Inc.
 * and is protected under U.S. and international copyright and other
 * intellectual property laws.
 *
 * DISCLAIMER:
 * This disclaimer is not a license and does not grant any rights to the
 * materials distributed herewith. Except as otherwise provided in a valid
 * license issued to you by Xilinx, and to the maximum extent permitted by
 * applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
 * FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
 * IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
 * MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
 * and (2) Xilinx shall not be liable (whether in contract or tort, including
 * negligence, or under any other theory of liability) for any loss or damage
 * of any kind or nature related to, arising under or in connection with these
 * materials, including for any direct, or any indirect, special, incidental,
 * or consequential loss or damage (including loss of data, profits, goodwill,
 * or any type of loss or damage suffered as a result of any action brought by
 * a third party) even if such damage or loss was reasonably foreseeable or
 * Xilinx had been advised of the possibility of the same.
 *
 * CRITICAL APPLICATIONS:
 * Xilinx products are not designed or intended to be fail-safe, or for use in
 * any application requiring fail-safe performance, such as life-support or
 * safety devices or systems, Class III medical devices, nuclear facilities,
 * applications related to the deployment of airbags, or any other applications
 * that could lead to death, personal injury, or severe property or
 * environmental damage (individually and collectively, "Critical
 * Applications"). Customer assumes the sole risk and liability of any use of
 * Xilinx products in Critical Applications, subject only to applicable laws
 * and regulations governing limitations on product liability.
 *
 * THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
 * AT ALL TIMES.
 *
 *******************************************************************************/

/*****************************************************************************
*
* @file hw_base.h
*
* Header containing the h/w base addresses and register offsets.
* Used by udriver.c file to access different h/w
*
******************************************************************************/

#ifndef HW_BASE_H_
#define HW_BASE_H_

#include "xparameters.h"

#define TPG_BASE			0x43C00000 //XPAR_TPG_0_BASEADDR <----- not in exported design
//#define VDMA_TPG_BASE		XPAR_TPG_VDMA_BASEADDR
//#define VDMA_FILTER_BASE	XPAR_FILTER_VDMA_BASEADDR
//#define FILTER_BASE			XPAR_IMAGE_FILTER_S_AXI_CONTROL_BUS_BASEADDR

#define SHIFT_16			16

/***** Macros *****/
#define REG_WRITE(addr, off, val) (*(volatile int*)(addr+off)=(val))
#define REG_READ(addr,off) (*(volatile int*)(addr+off))
#define REG_WRITE_T(addr, off, val, type) (*(volatile type*)(addr+off)=(val))
#define REG_READ_T(addr,off,type) (*(volatile type*)(addr+off))
#define SET_BIT(a,b) ((a) |= (1<<(b)))
#define CLR_BIT(a,b) ((a) &= ~(1<<(b)))
#define FLIP_BIT(a,b) ((a) ^= (1<<(b)))
#define CHECK_BIT(a,b) ((a) & (1<<(b)))

/***********  AXI_GPIO Registers ************/
#define GPIO_BASEADDR XPAR_AXI_GPIO_0_BASEADDR
#define GPIO_HIGHADDR XPAR_AXI_GPIO_0_HIGHADDR
#define GPIO_1_BASEADDR XPAR_AXI_GPIO_1_BASEADDR
#define GPIO_1_HIGHADDR XPAR_AXI_GPIO_1_HIGHADDR
#define GPIO_2_BASEADDR XPAR_AXI_GPIO_2_BASEADDR
#define GPIO_2_HIGHADDR XPAR_AXI_GPIO_2_HIGHADDR
#define GPIO_DATA_REG 0x00
#define GPIO_TRI_REG 0x04
#define GPIO2_DATA_REG 0x08
#define GPIO2_TRI_REG 0x0C

/***********  col_disc ************/
#define COLDISC_0_BASEADDR XPAR_COL_DISC_REG_0_S00_AXI_BASEADDR
#define COLDISC_0_HIGHADDR XPAR_COL_DISC_REG_0_S00_AXI_HIGHADDR
#define COLDISC_1_BASEADDR XPAR_COL_DISC_REG_1_S00_AXI_BASEADDR
#define COLDISC_1_HIGHADDR XPAR_COL_DISC_REG_1_S00_AXI_HIGHADDR

/***********  exposure ************/
#define EXPREG_0_BASEADDR XPAR_EXPOSURE_REG_0_S00_AXI_BASEADDR
#define EXPREG_0_HIGHADDR XPAR_EXPOSURE_REG_0_S00_AXI_HIGHADDR
#define EXPREG_1_BASEADDR XPAR_EXPOSURE_REG_1_S00_AXI_BASEADDR
#define EXPREG_1_HIGHADDR XPAR_EXPOSURE_REG_1_S00_AXI_HIGHADDR


/*********** VDMA Parameters ************/
#define FB0_ADDR 					0x17000000	//Frame buffer address for VDMA image storage
#define FB1_ADDR 					0x19000000	//Frame buffer address for VDMA image storage
#define FB2_ADDR 					0x1B000000	//Frame buffer address for VDMA image storage
#define FB3_ADDR 					0x1D000000	//Frame buffer address for VDMA image storage
#define FB4_ADDR 					0x1F000000	//Frame buffer address for VDMA image storage
#define FB5_ADDR					0x1F200000
//#define FB0_ADDR 					0x10000000	//Frame buffer address for VDMA image storage
//#define FB1_ADDR 					0x10400000	//Frame buffer address for VDMA image storage
//#define FB2_ADDR 					0x10C00000	//Frame buffer address for VDMA image storage
//#define FB3_ADDR 					0x11000000	//Frame buffer address for VDMA image storage

#define VDMA0_BASEADDR XPAR_AXIVDMA_0_BASEADDR
#define VDMA1_BASEADDR XPAR_AXIVDMA_1_BASEADDR
#define VDMA2_BASEADDR XPAR_AXIVDMA_2_BASEADDR
#define VDMA3_BASEADDR XPAR_AXIVDMA_3_BASEADDR
#define VDMA4_BASEADDR XPAR_AXIVDMA_4_BASEADDR

////(OLD!!)
//#define XAXIVDMA_CR_OFFSET    		0x00 /**< Channel control */
//#define XAXIVDMA_SR_OFFSET    		0x04 /**< Status */
//#define XAXIVDMA_CDESC_OFFSET 		0x08 /**< Current descriptor pointer */
//#define XAXIVDMA_TDESC_OFFSET		0x10 /**< Tail descriptor pointer */
//#define XAXIVDMA_HI_FRMBUF_OFFSET	0x14 /**< 32 FrameBuf Sel*/
//#define XAXIVDMA_FRMSTORE_OFFSET	0x18 /**< Frame Store */
//#define XAXIVDMA_BUFTHRES_OFFSET	0x1C /**< Line Buffer Thres */
//#define XAXIVDMA_MM2S_ADDR_OFFSET 	0x50 /**< MM2S channel Addr */
//#define XAXIVDMA_S2MM_ADDR_OFFSET 	0xA0 /**< S2MM channel Addr */
//
//#define XAXIVDMA_TX_OFFSET      	0x00 /**< TX channel registers base (MM2S VDMA Control Register) */
//#define XAXIVDMA_RX_OFFSET      	0x30 /**< RX channel registers base (S2MM VDMA Control Register) */
//#define XAXIVDMA_PARKPTR_OFFSET 	0x28 /**< Park Pointer Register */
//#define XAXIVDMA_VERSION_OFFSET 	0x2C /**< Version register */
//
//#define XAXIVDMA_VSIZE_OFFSET       0x00 /**< Vertical size */
//#define XAXIVDMA_HSIZE_OFFSET       0x04 /**< Horizontal size */
//#define XAXIVDMA_STRD_FRMDLY_OFFSET 0x08 /**< Stride and Frame delay */
//#define XAXIVDMA_START_ADDR_OFFSET  0x0C /**< Start of address */
//#define XAXIVDMA_START_ADDR_LEN     0x04 /**< Each entry is 4 bytes */

//VDMA From documentation
#define VDMA_MM2S_CTRL				0x00 //MM2S VDMA Control Register
#define VDMA_MM2S_STATUS			0x04 //MM2S VDMA Status Register
#define VDMA_MM2S_REG_INDEX			0x14 //MM2S Register Index

#define VDMA_PARK_PTR_REG			0x28 //MM2S and S2MM Park Pointer Register
#define VDMA_VERSION				0x2C //Video DMA Version Register

#define VDMA_S2MM_CTRL				0x30 //S2MM VDMA Control Register
#define VDMA_S2MM_STATUS			0x34 //S2MM VDMA Status Register
#define VDMA_S2MM_REG_INDEX			0x44 //S2MM Register Index

#define VDMA_MM2S_VSIZE				0x50 //MM2S Vertical Size Register
#define VDMA_MM2S_HSIZE				0x54 //MM2S Horizontal Size Register
#define VDMA_MM2S_FRMDLY_STRIDE		0x58 //MM2S Frame Delay and Stride Register
#define VDMA_MM2S_START_ADDRESS		0x5C //to 0x98 MM2S Start Address (1 to 16)

#define VDMA_S2MM_VSIZE				0xA0 //S2MM Vertical Size Register
#define VDMA_S2MM_HSIZE				0xA4 //S2MM Horizontal Size Register
#define VDMA_S2MM_FRMDLY_STRIDE		0xA8 //S2MM Frame Delay and Stride Register
#define VDMA_S2MM_START_ADDRESS		0xAC //to 0xE8  (1 to 16) S2MM Start Address (1 to 16)

#define VDMA_START_ADDR_LEN     	0x04 //Each entry is 4 bytes

/*********** TPG Parameters ************/
/* General Control Registers */
#define TPG_CONTROL 				0x000	//Control (R/W)
#define TPG_STATUS 					0x004	//Status (R/W)
#define TPG_ERROR 					0x008	//Error (R/W)
#define TPG_IRQ_EN 					0x00C	//IRQ Enable
#define TPG_VERSION             	0x010	//Version
/* Timing Control Registers */
#define TPG_ACTIVE_SIZE 			0x020
/* Core Specific Registers */
#define TPG_PATTERN_CONTROL 		0x100	//See below for values to input
#define TPG_MOTION_SPEED 			0x104
#define TPG_CROSS_HAIRS 			0x108
#define TPG_ZPLATE_HOR_CONTROL 		0x10C
#define TPG_ZPLATE_VER_CONTROL 		0x110
#define TPG_BOX_SIZE 				0x114
#define TPG_BOX_COLOR 				0x118
#define TPG_STUCK_PIXEL_THRESH 		0x11C
#define TPG_NOISE_GAIN 				0x120

/*********** Values for "TPG_PATTERN_CONTROL" ************/
#define PTRN_PASSTHROUGH	0x0	//Pass the video input straight through the video output
#define PTRN_HRAMP			0x1	//Horizontal Ramp which increases each component (RGB or CbCr) horizontally by 1
#define PTRN_VRAMP			0x2	//Vertical Ramp which increases each component (RGB or CbCr) vertically by 1
#define PTRN_TRAMP			0x3	/*Temporal Ramp which increases every pixel by a value set in the TPG_MOTION_SPEED register
								  for every frame. Luma (Y) stays at a fixed value of 2.*/
#define PTRN_RED			0x4	//Solid red output
#define PTRN_GREEN			0x5	//Solid green output
#define PTRN_BLUE			0x6	//Solid blue output
#define PTRN_BLACK			0x7	//Solid black output
#define PTRN_WHITE			0x8	//Solid white output
#define PTRN_COLOR_BARS		0x9	//Color bars
#define PTRN_WAT			0xA	/*Zone Plate output produces a ROM based sinusoidal pattern. This option has dependencies on
								  the TPG_MOTION_SPEED, TPG_ZPLATE_HOR_CONTR and TPG_ZPLATE_VER_CONTROL registers.*/
#define PTRN_TARTAN			0xB	//Tartan Color Bars
#define PTRN_HATCH			0xC	//Draws a cross hatch pattern.
#define PTRN_VHRAMP			0xE	//A combined vertical and horizontal ramp
#define PTRN_CHECKER		0xF	//Black and white checker


/******* TPG Control Register bit definition *******/
#define TPG_CTL_EN_MASK    0x00000001    /**< TPG Enable */
#define TPG_CTL_RUE_MASK   0x00000002    /**< TPG Register Update */
#define TPG_CTL_CS_MASK    0x00000004    /**< TPG Register Clear Status */

/******* TPG Reset Register bit definition *******/
#define TPG_RST_RESET      0x80000000    /**< Software Reset - Instantaneous */
#define TPG_RST_AUTORESET  0x40000000    /**< Software Reset - Auto-synchronize to SOF */

/*********** LogicVC (cvc) Parameters ************/
#define CVC_HSY_FP                           0x0000
#define CVC_HSY_WIDTH                        0x0008
#define CVC_HSY_BP                           0x0010
#define CVC_HSY_RESOLUTION                   0x0018
#define CVC_VSY_FP                           0x0020
#define CVC_VSY_WIDTH                        0x0028
#define CVC_VSY_BP                           0x0030
#define CVC_VSY_RESOLUTION                   0x0038
#define CVC_CTRL                             0x0040
#define CVC_DTYPE                            0x0048
#define CVC_BACKGROUND                       0x0050
#define CVC_VBUFF_SEL                        0x0058
#define CVC_CLUT_SEL                         0x0060
#define CVC_INT_STAT                         0x0068
#define CVC_INT_MASK                         0x0070
#define CVC_PWRCTRL                          0x0078
#define CVC_L0_H_OFFSET                      0x0100
#define CVC_L0_V_OFFSET                      0x0108
#define CVC_L0_H_POSITION                    0x0110
#define CVC_L0_V_POSITION                    0x0118
#define CVC_L0_WIDTH                         0x0120
#define CVC_L0_HEIGHT                        0x0128
#define CVC_L0_ALPHA                         0x0130
#define CVC_L0_CTRL                          0x0138
#define CVC_L0_TRANSPARENT                   0x0140
#define CVC_L1_H_OFFSET                      0x0180
#define CVC_L1_V_OFFSET                      0x0188
#define CVC_L1_H_POSITION                    0x0190
#define CVC_L1_V_POSITION                    0x0198
#define CVC_L1_WIDTH                         0x01A0
#define CVC_L1_HEIGHT                        0x01A8
#define CVC_L1_ALPHA                         0x01B0
#define CVC_L1_CTRL                          0x01B8
#define CVC_L1_TRANSPARENT                   0x01C0
#define CVC_L2_H_OFFSET                      0x0200
#define CVC_L2_V_OFFSET                      0x0208
#define CVC_L2_H_POSITION                    0x0210
#define CVC_L2_V_POSITION                    0x0218
#define CVC_L2_WIDTH                         0x0220
#define CVC_L2_HEIGHT                        0x0228
#define CVC_L2_ALPHA                         0x0230
#define CVC_L2_CTRL                          0x0238
#define CVC_L2_TRANSPARENT                   0x0240
#define CVC_L3_H_OFFSET                      0x0280
#define CVC_L3_V_OFFSET                      0x0288
#define CVC_L3_H_POSITION                    0x0290
#define CVC_L3_V_POSITION                    0x0298
#define CVC_L3_WIDTH                         0x02A0
#define CVC_L3_HEIGHT                        0x02A8
#define CVC_L3_ALPHA                         0x02B0
#define CVC_L3_CTRL                          0x02B8
#define CVC_L3_TRANSPARENT                   0x02C0
#define CVC_L4_CTRL                          0x0338


#endif /* HW_BASE_H_ */

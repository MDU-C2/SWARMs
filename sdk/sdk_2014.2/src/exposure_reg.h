/*
 * exposure_reg.h
 *
 *  Created on: 30 jun 2015
 *      Author: cag01
 */

#ifndef EXPOSURE_REG_H_
#define EXPOSURE_REG_H_

//#define EXPREG_BASEADDR 0x43C10000
//#define EXPREG_HIGHADDR 0x43C1FFFF
#define EXPREG0_OFFSET 0
#define EXPREG1_OFFSET 4
#define EXPREG2_OFFSET 8
#define EXPREG3_OFFSET 12

//    exposure_th <= slv_reg0(7 downto 0);
//    reg_data_out <= exposure_pos; --slv_reg1;
//    reg_data_out <= exposure_neg; --slv_reg2;



int expreg_set_th(uint8_t th, uint32_t base_addr, uint32_t high_addr);
int expreg_read(uint32_t *pos, uint32_t *neg, uint32_t base_addr, uint32_t high_addr);


#endif /* COL_DISC_H_ */

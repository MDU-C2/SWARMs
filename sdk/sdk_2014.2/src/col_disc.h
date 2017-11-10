/*
 * col_disc.h
 *
 *  Created on: 25 jun 2015
 *      Author: cag01
 */

#ifndef COL_DISC_H_
#define COL_DISC_H_

//#define COLDISC_BASEADDR 0x43C00000
//#define COLDISC_HIGHADDR 0x43C0FFFF
#define REG0_OFFSET 0
#define REG1_OFFSET 4
#define REG2_OFFSET 8
#define REG3_OFFSET 12

//    B <= slv_reg0(7 downto 0);
//    W <= slv_reg0(15 downto 8);
//    S <= slv_reg0(23 downto 16);
//    H1 <= slv_reg0(31 downto 24);
//    H2 <= slv_reg1(7 downto 0);
//    H3 <= slv_reg1(15 downto 8);
//    H4 <= slv_reg1(23 downto 16);
//    H5 <= slv_reg1(31 downto 24);
//    H6 <= slv_reg2(7 downto 0);

typedef struct
{
    uint8_t B;
    uint8_t W;
    uint8_t S;
    uint8_t H1;
    uint8_t H2;
    uint8_t H3;
    uint8_t H4;
    uint8_t H5;
    uint8_t H6;
} coldisc_th_t;

void coldisc_th_print(coldisc_th_t *th);
int coldisc_th_read(coldisc_th_t *th, uint32_t base_addr, uint32_t high_addr);
int coldisc_th_write(coldisc_th_t *th, uint32_t base_addr, uint32_t high_addr);


#endif /* COL_DISC_H_ */

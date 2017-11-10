/*
 * tpg.h
 *
 *  Created on:
 *      Author: Carl
 */

#ifndef TPG_H_
#define TPG_H_

void TPG_Start(uint32_t BaseAddress);
void TPG_Stop(uint32_t BaseAddress);

int TPG_Registers(uint32_t uBaseAddr);
int TPG_Errors(uint32_t BaseAdress);

void TPG_Set_Size(uint32_t BaseAddress, uint16_t h_size, uint16_t v_size);

#endif /* TPG_H_ */

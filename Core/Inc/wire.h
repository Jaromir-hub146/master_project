/*
 * wire.h
 *
 *  Created on: Sep 6, 2024
 *      Author: Jaromir
 */

#ifndef INC_WIRE_H_
#define INC_WIRE_H_

#include "stm32l4xx.h"

HAL_StatusTypeDef Wire_Reset(void);

uint8_t Wire_Read(void);

void Wire_Write(uint8_t byte);

uint8_t Wire_Crc(const uint8_t* data, int len);

#endif /* INC_WIRE_H_ */

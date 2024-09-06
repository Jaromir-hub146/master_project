/*
 * bme280_usage.h
 *
 *  Created on: Sep 6, 2024
 *      Author: Jaromir
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "stm32l4xx.h"

#define DS18B20_ROM_CODE_SIZE      8

HAL_StatusTypeDef DS18B20_Read_Address(uint8_t* rom_code);

HAL_StatusTypeDef DS18B20_Start_Measure(const uint8_t* rom_code);

float DS18B20_Get_Temp(const uint8_t* rom_code);

void Hex_Convert_ROM(uint8_t* rom, char* hex_rom, int length);

#endif /* INC_DS18B20_H_ */

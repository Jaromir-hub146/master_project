/*
 * bme280_usage.h
 *
 *  Created on: Aug 18, 2024
 *      Author: Jaromir
 */

#ifndef INC_BME280_USAGE_H_
#define INC_BME280_USAGE_H_

#include "bme280_defs.h"

int8_t I2C_Read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

int8_t I2C_Write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

int8_t BME280_Read_Data(struct bme280_data *received_data);

int8_t BME280_Init(void);

void BME280_Print_Data(struct bme280_data *rec_data);

void delay_ms(uint32_t period);

#endif /* INC_BME280_USAGE_H_ */

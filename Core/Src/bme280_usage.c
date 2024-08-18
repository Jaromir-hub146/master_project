/*
 * bme280_usage.c
 *
 *  Created on: Aug 18, 2024
 *      Author: Jaromir
 */

#include "bme280_usage.h"
#include "bme280.h"
#include "i2c.h"
#include "my_delay.h"
#include <stdio.h>

static struct bme280_dev bme_dev;

/**
 * @brief Read memory from register.
 *
 * If success function returns 0.
 *
 * @param
 */
int8_t I2C_Read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
	int8_t status = BME280_OK;
	HAL_I2C_Mem_Read(&hi2c1, dev_id, reg_addr, 1, reg_data, length, 100);
	return status;
}

int8_t I2C_Write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
	int8_t status = BME280_OK;
	HAL_I2C_Mem_Write(&hi2c1, dev_id, reg_addr, 1, reg_data, length, 100);
	return status;
}

int8_t BME280_Init(void)
{
	int8_t status = BME280_OK;
	uint8_t dev_settings;

	bme_dev.dev_id = (BME280_I2C_ADDR_PRIM<<1);
	bme_dev.intf = BME280_I2C_INTF;
	bme_dev.read = I2C_Read;
	bme_dev.write = I2C_Write;
	bme_dev.delay_ms = DELAY_ms;

	status = bme280_init(&bme_dev);

	if(status == BME280_OK)
	{

		/* Recommended mode of operation: Indoor navigation */
		bme_dev.settings.osr_h = BME280_OVERSAMPLING_1X;
		bme_dev.settings.osr_p = BME280_OVERSAMPLING_16X;
		bme_dev.settings.osr_t = BME280_OVERSAMPLING_2X;
		bme_dev.settings.filter = BME280_FILTER_COEFF_16;
		bme_dev.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

		dev_settings = BME280_OSR_PRESS_SEL;
		dev_settings |= BME280_OSR_TEMP_SEL;
		dev_settings |= BME280_OSR_HUM_SEL;
		dev_settings |= BME280_STANDBY_SEL;
		dev_settings |= BME280_FILTER_SEL;

		status = bme280_set_sensor_settings(dev_settings, &bme_dev);

		if(status == BME280_OK)
		{
			status = bme280_set_sensor_mode(BME280_NORMAL_MODE, &bme_dev);
		}
	}

	return status;
}

int8_t BME280_Read_Data(struct bme280_data *received_data)
{
	int8_t status = BME280_OK;

	status = bme280_get_sensor_data(BME280_ALL, received_data, &bme_dev);
	bme_dev.delay_ms(100);

	return status;
}

void BME280_Print_Data(struct bme280_data *bme_data)
{
	printf("Temperature, Pressure, Humidity\r\n");
#ifdef BME280_FLOAT_ENABLE
	printf("%0.2f, %0.2f, %0.2f\r\n", bme_data->temperature, bme_data->pressure, bme_data->humidity);
#else
	printf("%d, %u, %u\r\n", bme_data->temperature, bme_data->pressure, bme_data->humidity);
#endif
}



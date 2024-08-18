/*
 * my_delay.c
 *
 *  Created on: Aug 18, 2024
 *      Author: Jaromir
 */

#include "my_delay.h"
#include "tim.h"

void DELAY_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	while(__HAL_TIM_GET_COUNTER(&htim6) < us) {}
}

void DELAY_ms(uint16_t ms)
{
	uint16_t counter = 0;

	while (counter < ms)
	{
		DELAY_us(1000);
		counter++;
	}
}

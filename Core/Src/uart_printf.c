/*
 * uart_printf.c
 *
 *  Created on: Aug 18, 2024
 *      Author: Jaromir
 */

#include "usart.h"

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, 1000);
	return ch;
}

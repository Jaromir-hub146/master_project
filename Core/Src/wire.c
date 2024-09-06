/*
 * wire.c
 *
 *  Created on: Sep 6, 2024
 *      Author: Jaromir
 */

#include "wire.h"
#include "usart.h"

static void Set_Baud_Rate(uint32_t baudrate)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = baudrate;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  huart4.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  if (HAL_HalfDuplex_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
}

HAL_StatusTypeDef Wire_Reset(void)
{
    uint8_t data_tx = 0xF0;
    uint8_t data_rx = 0;

    Set_Baud_Rate(9600);
    HAL_UART_Transmit(&huart4, &data_tx, 1, HAL_MAX_DELAY);
    HAL_UART_Receive(&huart4, &data_rx, 1, HAL_MAX_DELAY);
    Set_Baud_Rate(115200);

    if (data_rx != 0xF0)
        return HAL_OK;
    else
        return HAL_ERROR;
}

static int Read_Bit(void)
{
    uint8_t data_tx = 0xFF;
    uint8_t data_rx = 0;
    HAL_UART_Transmit(&huart4, &data_tx, 1, HAL_MAX_DELAY);
    HAL_UART_Receive(&huart4, &data_rx, 1, HAL_MAX_DELAY);

    return data_rx & 0x01; // read last significant bit
}

uint8_t Wire_Read(void)
{
    uint8_t received = 0;
    for (int i = 0; i < 8; i++) {
        received >>= 1;
        if (Read_Bit())
            received |= 0x80;
    }
    return received;
}

static void Write_Bit(int value)
{
    if (value) {
        uint8_t data_tx = 0xff;
        HAL_UART_Transmit(&huart4, &data_tx, 1, HAL_MAX_DELAY);
    } else {
        uint8_t data_tx = 0x0;
        HAL_UART_Transmit(&huart4, &data_tx, 1, HAL_MAX_DELAY);
    }
}

void Wire_Write(uint8_t byte)
{
    for (int i = 0; i < 8; i++) {
        Write_Bit(byte & 0x01);
        byte >>= 1;
    }
}

uint8_t Wire_Crc(const uint8_t* data, int length)
{
    uint8_t crc_code = 0;
 	 while (length--) {

 		 uint8_t byte = *data; //byte = *(data++) ->
 		 data = data + 1;

 		 for (int i = 0; i<8; i++) {
 			 	 uint8_t val = (crc_code ^ byte);
 			 	 val = val & 0x01;
 			 	 crc_code >>= 1;
 			 	 if (val) {
 			 		 crc_code ^= 0x8C;
 			    }
 			 	 byte >>= 1;
 		}

 	}
 	 return crc_code;
}
/*
 * ds18b20.c
 *
 *  Created on: Sep 6, 2024
 *      Author: Jaromir
 */

#include <string.h>
#include "ds18b20.h"
#include "wire.h"

#define DS18B20_SCRATCHPAD_SIZE      9

#define DS18B20_READ_ROM           0x33
#define DS18B20_MATCH_ROM          0x55
#define DS18B20_SKIP_ROM           0xCC
#define DS18B20_CONVERT_T          0x44
#define DS18B20_READ_SCRATCHPAD    0xBE

/**
 * @brief Allows to set baud rate of UART transmission.
 *
 * TODO.
 *
 * @param
 */

HAL_StatusTypeDef DS18B20_Read_Address(uint8_t* rom_code)
{
  int status = HAL_OK;
  uint8_t crc;

  if (Wire_Reset() != HAL_OK)
    status = HAL_ERROR;

  if (status != HAL_ERROR) {
    Wire_Write(DS18B20_READ_ROM);

    for (int i = 0; i < DS18B20_ROM_CODE_SIZE; i++)
    {
      rom_code[i] = Wire_Read();
    }

    crc = Wire_Crc(rom_code, DS18B20_ROM_CODE_SIZE - 1);
    
    if(rom_code[DS18B20_ROM_CODE_SIZE - 1] == crc)
      status = HAL_OK;
    else 
      status = HAL_ERROR;
    
    return status;
  }

  return status;
}

static HAL_StatusTypeDef Send_CMD(const uint8_t* rom_code, uint8_t command)
{
  int status = HAL_OK;

  if (Wire_Reset() != HAL_OK)
    status = HAL_ERROR;

  if (status != HAL_ERROR) {

    if(rom_code == NULL) {
      Wire_Write(DS18B20_SKIP_ROM);
    } else {
      Wire_Write(DS18B20_MATCH_ROM);
      
      for (int i = 0; i < DS18B20_ROM_CODE_SIZE; i++)
        Wire_Write(rom_code[i]);
    }

    Wire_Write(command);
    
    return status;
  }

  return status;
}

HAL_StatusTypeDef DS18B20_Start_Measure(const uint8_t* rom_code)
{
  return Send_CMD(rom_code, DS18B20_CONVERT_T);
}

static HAL_StatusTypeDef DS18B20_Read_Scratchpad(const uint8_t* rom_code, uint8_t* scratchpad)
{
  int status = HAL_OK;
  uint8_t crc;

  if (Send_CMD(rom_code, DS18B20_READ_SCRATCHPAD) != HAL_OK)
    status = HAL_ERROR;

  if (status != HAL_ERROR) {

    for (int i = 0; i < DS18B20_SCRATCHPAD_SIZE; i++)
    {
      scratchpad[i] = Wire_Read();
    }

    crc = Wire_Crc(scratchpad, DS18B20_SCRATCHPAD_SIZE - 1);
    
    if (scratchpad[DS18B20_SCRATCHPAD_SIZE - 1] == crc)
      status = HAL_OK;
    else 
      status = HAL_ERROR;
    
    return status;
  }

  return status;
}

float DS18B20_Get_Temp(const uint8_t* rom_code)
{
  uint8_t scratchpad[DS18B20_SCRATCHPAD_SIZE];
  int16_t temp;

  if (DS18B20_Read_Scratchpad(rom_code, scratchpad) != HAL_OK)
    return 85.0f; // default wrong value

  memcpy(&temp, &scratchpad[0], sizeof(temp));

  return temp / 16.0f;
}

/**
 * @brief Converts rom value to hex representation.
 *
 * TODO.
 *
 * @param
 */

void Hex_Convert_ROM(uint8_t* rom, char* hex_rom, int length)
{
  uint8_t decnum;
  uint8_t rem;
  char hexnum[16];
  int j = 0;
  for (int i = 0; i<8; i++){
    decnum = rom[i];
    int k = 0;
    if (decnum == 0){
      hexnum[j] = 48;
      j++;
      hexnum[j] =48;
      j++;
    }
    while(decnum!=0)
    {
      rem = decnum%16;
      if(rem<10)
        rem = rem+48;
      else
        rem = rem+55;
      hexnum[j] = rem;
      j++;
      decnum = decnum/16;
      k++;
      if (decnum == 0 && k < 2){
        hexnum[j] = 0 + 48;
        j++;
      }

    }
  }

  int space = 32;
  int x_ = 88;
  for (int c = 0; c < length/5; c++) {

    hex_rom[5*c] = 48;
    hex_rom[5*c+1] = x_;
    hex_rom[5*c+2] = hexnum[2*c+1];
    hex_rom[5*c+3] = hexnum[2*c];
    hex_rom[5*c+4] = space;

  }
}


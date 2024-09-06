/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "DFRobot_BloodOxygen_S.hpp"
#include "fonts.h"
#include "ssd1306.h"
extern "C" {
#include "bme280_usage.h"
#include "bme280_defs.h"
#include "my_delay.h"
#include "ds18b20.h"
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static struct bme280_data bme_received_data;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
// TIMER
  HAL_TIM_Base_Start(&htim6);
// MAX30102
  DFRobot_BloodOxygen_S_I2C MAX30102(0x57);
  MAX30102.sensorStartCollect();
// BME280
  if(BME280_Init() != BME280_OK)
  {
	  printf("Blad inicjalizacji BME280!\n");
  }
// DS18B20
  uint8_t ds18b20_rom_code[DS18B20_ROM_CODE_SIZE];
  if (DS18B20_Read_Address(ds18b20_rom_code) != HAL_OK) {
    printf("Blad inicjalizacji DS18B20!\r\n");
    Error_Handler();
  }
//SSD1306
  SSD1306_Init();
  SSD1306_GotoXY (0,0);
  SSD1306_Puts("STARTING", &Font_11x18, (SSD1306_COLOR_t)1);
  SSD1306_GotoXY (0, 30);
  SSD1306_Puts("DEVICE", &Font_11x18, (SSD1306_COLOR_t)1);
  SSD1306_UpdateScreen();
  DELAY_ms (1000);
  SSD1306_ScrollRight(0,7);
  DELAY_ms(3000);
  SSD1306_ScrollLeft(0,7);
  DELAY_ms(3000);
  SSD1306_Stopscroll();
  SSD1306_Clear();

//  int num=2024;
//  char snum[5];
//  SSD1306_GotoXY (30,20);
//  itoa(num, snum, 10);
//  SSD1306_Puts (snum, &Font_16x26, (SSD1306_COLOR_t)1);
//  SSD1306_UpdateScreen();
//
//  //SSD1306_DrawLine(POINT1 X, POINT1 Y, POINT2 X, POINT2 Y, 1);
//  SSD1306_DrawLine(1,30,126,30, (SSD1306_COLOR_t)1);
//  SSD1306_UpdateScreen();
//  DELAY_ms (5000);
//  SSD1306_Clear();
//
//  // For Rectangle, we need to use two corner points
//  // SSD1306_DrawRectangle(POINT1 X, POINT1 Y, POINT2 X, POINT2 Y, 1);
//  SSD1306_DrawRectangle(17,1,115,14, (SSD1306_COLOR_t)1);
//  // SSD1306_DrawTriangle(POINT1X, POINT1Y, POINT2X, POINT2Y, POINT3X, POINT3Y, 1);
//  SSD1306_DrawTriangle(73,22,124,62,74,54, (SSD1306_COLOR_t)1);
//  // SSD1306_DrawCircle(CENTER POINT X, CENTER POINT Y, RADIUS, 1);
//  SSD1306_DrawCircle(34,50,13, (SSD1306_COLOR_t)1);
//  SSD1306_UpdateScreen();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
// MAX30102
  MAX30102.getHeartbeatSPO2();
  printf("SpO2 = %d, Heart Rate: %d, Board temperature: %0.2f \r\n", MAX30102._sHeartbeatSPO2.SPO2, MAX30102._sHeartbeatSPO2.Heartbeat, MAX30102.getTemperature_C());
  DELAY_ms(2000);

// DS18B20
  DS18B20_Start_Measure(NULL);

  DELAY_ms(750);

  float temperature = DS18B20_Get_Temp(NULL);

  if (temperature >= 80.0f) {
    printf("DS18B20 ERROR!\r\n");
  } else {
    printf("Temperature = %0.2fC\r\n", temperature);
  }
//SSD1306
  char max30102_oled_sp02[12];
  sprintf(max30102_oled_sp02, "SpO2: %d", MAX30102._sHeartbeatSPO2.SPO2);

  char max30102_oled_hr[15];
  sprintf(max30102_oled_hr, "Pulse: %d", MAX30102._sHeartbeatSPO2.Heartbeat);

  char temp_oled[15];
  sprintf(temp_oled, "Temp: %0.2fC", temperature);

  SSD1306_Clear();
  SSD1306_GotoXY (0,0);
  SSD1306_Puts(max30102_oled_sp02, &Font_11x18, (SSD1306_COLOR_t)1);
  SSD1306_GotoXY (0, 24);
  SSD1306_Puts(max30102_oled_hr, &Font_11x18, (SSD1306_COLOR_t)1);
  SSD1306_GotoXY (0, 45);
  SSD1306_Puts(temp_oled, &Font_11x18, (SSD1306_COLOR_t)1);
  SSD1306_UpdateScreen();

// BME280
	if(BME280_Read_Data(&bme_received_data) != BME280_OK)
	{
		printf("Blad odczytu BME280!\r\n");
	}
	else
	{
		BME280_Print_Data(&bme_received_data);
	}

	DELAY_ms(1000);
   }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define W5500_SCSn_Pin GPIO_PIN_12
#define W5500_SCSn_GPIO_Port GPIOB
#define W5500_SCLK_Pin GPIO_PIN_13
#define W5500_SCLK_GPIO_Port GPIOB
#define W5500_MISO_Pin GPIO_PIN_14
#define W5500_MISO_GPIO_Port GPIOB
#define W5500_MOSI_Pin GPIO_PIN_15
#define W5500_MOSI_GPIO_Port GPIOB
#define Relay1_Pin GPIO_PIN_8
#define Relay1_GPIO_Port GPIOD
#define Relay2_Pin GPIO_PIN_9
#define Relay2_GPIO_Port GPIOD
#define Relay3_Pin GPIO_PIN_10
#define Relay3_GPIO_Port GPIOD
#define Relay4_Pin GPIO_PIN_11
#define Relay4_GPIO_Port GPIOD
#define Relay5_Pin GPIO_PIN_12
#define Relay5_GPIO_Port GPIOD
#define Relay6_Pin GPIO_PIN_13
#define Relay6_GPIO_Port GPIOD
#define Relay7_Pin GPIO_PIN_14
#define Relay7_GPIO_Port GPIOD
#define Relay8_Pin GPIO_PIN_15
#define Relay8_GPIO_Port GPIOD
#define W5500_RST_Pin GPIO_PIN_9
#define W5500_RST_GPIO_Port GPIOC
#define W5500_INTn_Pin GPIO_PIN_8
#define W5500_INTn_GPIO_Port GPIOA
#define W5500_PMODE0_Pin GPIO_PIN_5
#define W5500_PMODE0_GPIO_Port GPIOB
#define W5500_PMODE1_Pin GPIO_PIN_6
#define W5500_PMODE1_GPIO_Port GPIOB
#define W5500_PMODE2_Pin GPIO_PIN_7
#define W5500_PMODE2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define STDIN_FILENO    0       /* standard input file descriptor */
#define STDOUT_FILENO   1       /* standard output file descriptor */
#define STDERR_FILENO   2       /* standard error file descriptor */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

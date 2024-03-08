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
#include "socket.h"
#include "control.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

#define DATA_BUF_SIZE   2048

typedef struct
{
    uint8_t macaddr[6];
    uint8_t target_ipaddr[4];
    uint8_t target_port[2];
    uint8_t local_mode;
    uint8_t local_ipaddr[4];
    uint8_t local_gwaddr[4];
    uint8_t local_netmask[4];
    uint8_t local_port[2];
    uint8_t usart_para[4];
    uint8_t crc;
} __packed userparameter_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern userparameter_t user_config;
extern uint8_t config_valid_flag;
extern uint8_t gDATABUF[DATA_BUF_SIZE];
extern wiz_NetInfo gWIZNETINFO;

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define W5500_PMODE0_Pin GPIO_PIN_5
#define W5500_PMODE0_GPIO_Port GPIOE
#define W5500_PMODE1_Pin GPIO_PIN_6
#define W5500_PMODE1_GPIO_Port GPIOE
#define W5500_PMODE2_Pin GPIO_PIN_13
#define W5500_PMODE2_GPIO_Port GPIOC
#define W5500_SCSn_Pin GPIO_PIN_4
#define W5500_SCSn_GPIO_Port GPIOA
#define W5500_SCLK_Pin GPIO_PIN_5
#define W5500_SCLK_GPIO_Port GPIOA
#define W5500_MISO_Pin GPIO_PIN_6
#define W5500_MISO_GPIO_Port GPIOA
#define W5500_MOSI_Pin GPIO_PIN_7
#define W5500_MOSI_GPIO_Port GPIOA
#define Relay1_Pin GPIO_PIN_0
#define Relay1_GPIO_Port GPIOB
#define W5500_RST_Pin GPIO_PIN_1
#define W5500_RST_GPIO_Port GPIOB
#define Relay2_Pin GPIO_PIN_12
#define Relay2_GPIO_Port GPIOB
#define Relay3_Pin GPIO_PIN_13
#define Relay3_GPIO_Port GPIOB
#define Relay4_Pin GPIO_PIN_14
#define Relay4_GPIO_Port GPIOB
#define Relay5_Pin GPIO_PIN_15
#define Relay5_GPIO_Port GPIOB
#define W5500_INTn_Pin GPIO_PIN_8
#define W5500_INTn_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define INFO_OUTPUT
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

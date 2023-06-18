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
#include "stm32wbxx_hal.h"

#include "app_conf.h"
#include "app_entry.h"
#include "app_common.h"

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
#define POW_STAT_INPUT_Pin GPIO_PIN_13
#define POW_STAT_INPUT_GPIO_Port GPIOC
#define FAN_OUTPUT_Pin GPIO_PIN_8
#define FAN_OUTPUT_GPIO_Port GPIOB
#define DISP_CS_OUTPUT_Pin GPIO_PIN_9
#define DISP_CS_OUTPUT_GPIO_Port GPIOB
#define U_OUT1_ADC1_IN1_Pin GPIO_PIN_0
#define U_OUT1_ADC1_IN1_GPIO_Port GPIOC
#define U_OUT2_ADC1_IN2_Pin GPIO_PIN_1
#define U_OUT2_ADC1_IN2_GPIO_Port GPIOC
#define U_OUT3_ADC1_IN3_Pin GPIO_PIN_2
#define U_OUT3_ADC1_IN3_GPIO_Port GPIOC
#define C_OUT_ADC1_IN6_Pin GPIO_PIN_1
#define C_OUT_ADC1_IN6_GPIO_Port GPIOA
#define BAT_U_ADC1_IN7_Pin GPIO_PIN_2
#define BAT_U_ADC1_IN7_GPIO_Port GPIOA
#define TS_CS_OUTPUT_Pin GPIO_PIN_3
#define TS_CS_OUTPUT_GPIO_Port GPIOA
#define FRAM_CS_OUTPUT_Pin GPIO_PIN_4
#define FRAM_CS_OUTPUT_GPIO_Port GPIOA
#define GEN_ON_OUTPUT_Pin GPIO_PIN_4
#define GEN_ON_OUTPUT_GPIO_Port GPIOC
#define GEN_OFF_OUTPUT_Pin GPIO_PIN_5
#define GEN_OFF_OUTPUT_GPIO_Port GPIOC
#define RL_ON3_OUTPUT_Pin GPIO_PIN_2
#define RL_ON3_OUTPUT_GPIO_Port GPIOB
#define INV_ON_OUTPUT_Pin GPIO_PIN_10
#define INV_ON_OUTPUT_GPIO_Port GPIOB
#define INV_OFF_OUTPUT_Pin GPIO_PIN_11
#define INV_OFF_OUTPUT_GPIO_Port GPIOB
#define RL_ON1_OUTPUT_Pin GPIO_PIN_0
#define RL_ON1_OUTPUT_GPIO_Port GPIOB
#define RL_ON2_OUTPUT_Pin GPIO_PIN_1
#define RL_ON2_OUTPUT_GPIO_Port GPIOB
#define BAT_DONE_INPUT_Pin GPIO_PIN_12
#define BAT_DONE_INPUT_GPIO_Port GPIOB
#define BAT_CHRG_INPUT_Pin GPIO_PIN_13
#define BAT_CHRG_INPUT_GPIO_Port GPIOB
#define GEN_START_OUTPUT_Pin GPIO_PIN_6
#define GEN_START_OUTPUT_GPIO_Port GPIOC
#define LED1_OUTPUT_Pin GPIO_PIN_0
#define LED1_OUTPUT_GPIO_Port GPIOD
#define LED2_OUTPUT_Pin GPIO_PIN_1
#define LED2_OUTPUT_GPIO_Port GPIOD
#define ONE_WIRE_Pin GPIO_PIN_6
#define ONE_WIRE_GPIO_Port GPIOB
#define BUZ_OUTPUT_Pin GPIO_PIN_7
#define BUZ_OUTPUT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define APP_VERSION_NUMBER  "V0.0.1"
#define APP_VERSION_MAJOR   0
#define APP_VERSION_MINOR   0
#define APP_VERSION_BUILD   1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : custom_app.h
 * Description        : Header for custom_app.c module
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CUSTOM_APP_H
#define __CUSTOM_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_ble.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  CUSTOM_CONN_HANDLE_EVT,
  CUSTOM_DISCON_HANDLE_EVT,
  CUSTOM_UPDATE_FRAME_HANDLE_EVT,
} Custom_App_Opcode_Notification_evt_t;

typedef enum
{
  CUSTOM_APP_ERROR_OK				= 0x80,
  CUSTOM_APP_ERROR_NOT_FOUND_DATA  	= 0x81,
  CUSTOM_APP_ERROR_NOT_FOUND_HANDLE = 0x82,

  CUSTOM_APP_ERROR_UNKNOW			= 0x9F
} Custom_App_Error_t;  //0x80 ... 0x9F: Application Error

typedef struct
{
  Custom_App_Opcode_Notification_evt_t	Custom_Evt_Opcode;
  uint16_t                              ConnectionHandle;
} Custom_App_ConnHandle_Not_evt_t;

/* USER CODE BEGIN ET */
typedef struct
{
	void (*Events)(Custom_App_Opcode_Notification_evt_t code);
	void (*Resquest)(uint8_t *data, uint8_t Len);
	void (*Response)(uint8_t *data, uint8_t *Len);
} tCustomAppNotification;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */


/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ---------------------------------------------*/
void Custom_APP_Init(tCustomAppNotification EventHandle);

void Custom_APP_Notification(const Custom_App_ConnHandle_Not_evt_t *pNotification);

/* USER CODE BEGIN EF */
void Custom_APP_Send_Notification(uint8_t *data, uint8_t Len);

void Custom_APP_Context_UpdateDataFrame(uint8_t *data, uint8_t Len);

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*__CUSTOM_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

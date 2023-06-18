/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : custom_stm.h
  * Description        : Header for custom_stm.c module.
  *
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
#ifndef __CUSTOM_STM_H
#define __CUSTOM_STM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ble_types.h"

#define MAX_CHAR_LEN		247									/* Based on CubeMX */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  /* ELECTRICFENCE */
  CUSTOM_STM_WRITE_DATAFRAME,
  CUSTOM_STM_DATASTATUS,
  CUSTOM_STM_NOTIFY_DATAFRAME,
} Custom_STM_Char_Opcode_t;

typedef enum
{
	/* DataFrame */
	CUSTOM_STM_DATAFRAME_READ_EVT,
	CUSTOM_STM_DATAFRAME_WRITE_EVT,
	CUSTOM_STM_DATAFRAME_NOTIFY_ENABLED_EVT,
	CUSTOM_STM_DATAFRAME_NOTIFY_DISABLED_EVT,
	/* DataStatus */
	CUSTOM_STM_DATASTATUS_READ_EVT,
	CUSTOM_STM_DATASTATUS_NOTIFY_ENABLED_EVT,
	CUSTOM_STM_DATASTATUS_NOTIFY_DISABLED_EVT,

	CUSTOM_STM_BOOT_REQUEST_EVT,

	CUSTOM_STM_PERMIT_READ_EVT,

} Custom_STM_Opcode_evt_t;

typedef struct
{
  uint8_t *pPayload;
  uint8_t  Length;
} Custom_STM_Data_t;

typedef struct
{
  Custom_STM_Opcode_evt_t	Custom_Evt_Opcode;
  Custom_STM_Data_t         DataTransfered;
  uint16_t                  ConnectionHandle;
  uint8_t                   ServiceInstance;
} Custom_STM_App_Notification_evt_t;

/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void Custom_STM_Service_Init( void );
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification);
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload, uint8_t PayloadLen);

/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*__CUSTOM_STM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
